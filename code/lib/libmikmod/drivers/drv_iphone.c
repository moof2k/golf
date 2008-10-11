/*	MikMod sound library
	(c) 1998, 1999, 2000 Miodrag Vallat and others - see file AUTHORS for
	complete list.

	This library is free software; you can redistribute it and/or modify
	it under the terms of the GNU Library General Public License as
	published by the Free Software Foundation; either version 2 of
	the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Library General Public License for more details.
 
	You should have received a copy of the GNU Library General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
	02111-1307, USA.
*/

/*==============================================================================

  $Id $

  Driver for iPhone / iPodTouch (AudioQueue) (c) 2008 Damien Ciabrini

==============================================================================*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <AudioToolbox/AudioQueue.h>

#include <stdlib.h>

#include "mikmod_internals.h"

bool gMikModPaused = false;

#define ON_ERROR(x) do {			\
    if ( err != 0 ) {				\
      _mm_errno = (x);				\
      return 1;					\
    }						\
} while (0)
    

/**
 * Function in charge of feeding the AudioQueue with samples
 */
static BOOL iPhoneDrv_FillAudioBuffer(AudioQueueBufferRef mBuffer);

/**
 * Callback invoked by AudioQueue when a buffer becomes available
 */
static void iPhoneDrv_AudioCallback(void *data, 
				    AudioQueueRef mQueue, 
				    AudioQueueBufferRef mBuffer);


/**
 * Default number of buffers. Double buffer scheme
 */
static int mNbBuffers=2;

/**
 * Default size of a buffer in bytes
 */
static int mBufferSizeInBytes=40000;


/**
 * Default size of a buffer in bytes
 */
static float mVolume=1.0;

/**
 * The AudioQueue
 * Not static so that external ObjC code can control volume
 */
MIKMODAPI AudioQueueRef mMikModAudioQueue;

/**
 * The AudioQueue buffers
 */
static AudioQueueBufferRef *mBuffers;

/**
 * The next buffer to fill for playback
 */
static AudioQueueBufferRef mNextBuffer;

/**
 * simple marker to prevent any locking issue with the audio callback
 */
static int mQueueIsBeingStopped;




static void iPhoneDrv_CommandLine(CHAR *cmdline)
{
    CHAR *nb = MD_GetAtom("buffers", cmdline, 0);
    if ( nb!=NULL ) {
	mNbBuffers = atoi(nb);
    }

    CHAR *size = MD_GetAtom("size", cmdline, 0);
    if ( size!=NULL ) {
	mBufferSizeInBytes = atoi(size);
    }

    CHAR *volume = MD_GetAtom("volume", cmdline, 0);
    if ( volume!=NULL ) {
      mVolume = (float)atof(volume);
    }
}

static BOOL iPhoneDrv_IsThere(void)
{
    return 1;
}

static BOOL iPhoneDrv_Init(void)
{
    AudioStreamBasicDescription mDataFormat;
    UInt32 err;
    UInt32 i;

    /* We force this format for iPhone */
    mDataFormat.mFormatID = kAudioFormatLinearPCM;
    mDataFormat.mFormatFlags = kLinearPCMFormatFlagIsSignedInteger |
	kAudioFormatFlagIsPacked;

    /* User defined playback settings */
    if ((md_mixfreq == 44100) ||
	(md_mixfreq == 22050) ||
	(md_mixfreq == 11025) ||
	(md_mixfreq == 8000)) {
      mDataFormat.mSampleRate = md_mixfreq;
    } else {
      mDataFormat.mSampleRate = 44100;
    }

    if (md_mode & DMODE_16BITS) {
      mDataFormat.mBitsPerChannel = 16;
    } else {
      mDataFormat.mBitsPerChannel = 8;
    }

    if (md_mode & DMODE_STEREO) {
      mDataFormat.mChannelsPerFrame = 2;
    } else {
      mDataFormat.mChannelsPerFrame = 1;
    }

    mDataFormat.mBytesPerFrame = 
      (mDataFormat.mBitsPerChannel>>3) * mDataFormat.mChannelsPerFrame;

    mDataFormat.mFramesPerPacket = 1; 
    mDataFormat.mBytesPerPacket = mDataFormat.mBytesPerFrame;
    
    /* Create an Audio Queue... */
    err = AudioQueueNewOutput( &mDataFormat, 
			       iPhoneDrv_AudioCallback, NULL, 
			       CFRunLoopGetCurrent(), kCFRunLoopCommonModes,
			       0, 
			       &mMikModAudioQueue );
    ON_ERROR( MMERR_IPHONE_CREATEQUEUE );
 
    /* ... and its associated buffers */
    mBuffers = malloc( sizeof(AudioQueueBufferRef) * mNbBuffers );

    for (i=0; i<mNbBuffers; i++) {
	AudioQueueBufferRef mBuffer;

	err = AudioQueueAllocateBuffer( mMikModAudioQueue, mBufferSizeInBytes, &mBuffer );
	ON_ERROR( MMERR_IPHONE_ALLOCBUFFER );
	
	mBuffers[i]=mBuffer;
    }

    /* Set initial playback volume */
    err = AudioQueueSetParameter( mMikModAudioQueue, kAudioQueueParam_Volume, mVolume );
    ON_ERROR( MMERR_IPHONE_VOLUME );

    /* MikMod stuff */
    md_mode |= DMODE_SOFT_MUSIC | DMODE_SOFT_SNDFX;

    return VC_Init();
}


static void iPhoneDrv_Exit(void)
{
    AudioQueueDispose( mMikModAudioQueue, true );
    free( mBuffers );
}


static BOOL iPhoneDrv_PlayStart(void)
{
    UInt32 err;
    UInt32 i;

    VC_PlayStart();	
    
    /*
     * Enqueue all the allocated buffers before starting the playback.
     * The audio callback will be called as soon as one buffer becomes
     * available for filling.
     */
    for (i=0; i<mNbBuffers; i++) {
	iPhoneDrv_FillAudioBuffer( mBuffers[i] );
    }

    /* Start the Audio Queue! */
    err = AudioQueueStart( mMikModAudioQueue, NULL );
    ON_ERROR( MMERR_IPHONE_PLAYBACK );

    return 0;
}


static void iPhoneDrv_PlayStop(void)
{
    mQueueIsBeingStopped = TRUE;

    AudioQueueStop( mMikModAudioQueue, TRUE );

    mQueueIsBeingStopped = FALSE;

    VC_PlayStop();

}


static void iPhoneDrv_Update(void)
{
    /* the real processing takes place in FillAudioBuffer */
    iPhoneDrv_FillAudioBuffer( mNextBuffer );
}


static BOOL iPhoneDrv_FillAudioBuffer(AudioQueueBufferRef mBuffer)
{
    UInt32 err;

    if ( !mQueueIsBeingStopped ) {
		if(!gMikModPaused)
		{
			int nbBytes = VC_WriteBytes( mBuffer->mAudioData, mBufferSizeInBytes );
			mBuffer->mAudioDataByteSize = nbBytes;
			
			err = AudioQueueEnqueueBuffer( mMikModAudioQueue, mBuffer, 0, NULL);
			ON_ERROR( MMERR_IPHONE_ENQUEUEBUFFER );
		}
    }

    return 0;
}


void iPhoneDrv_AudioCallback(void *data, 
			     AudioQueueRef mQueue,
			     AudioQueueBufferRef mBuffer)
{
    /* Pass the current audio buffer to the iPhone driver */
    mNextBuffer = mBuffer;

    /* Inside the callback, one must not call any MikMod function
     * while the AudioQueue is being flused, because the driver's
     * PlayStop function has a lock on Mikmod's mutex.
     *
     * Calling MikMod_Update while the mutex is held would block this
     * callback, which would prevent the AudioQueue from stopping and
     * would lead to a dead lock situation.
     */
    if ( !mQueueIsBeingStopped ) {
	/* Tell Mikmod to fill the available audio buffer */
		if(!gMikModPaused)
			MikMod_Update();
    }
}


MIKMODAPI MDRIVER iPhoneDrv = {
    NULL,
    "iPhone",
    "iPhone/iPodTouch AudioQueue driver v0.1",
    0,255,
    "iphone",
    "buffers:t:2:Number of audio buffers\n"
    "size:t:40000:Buffer size in bytes\n"
    "volume:t:1.0:Default AudioQueue volume\n",
    iPhoneDrv_CommandLine,
    iPhoneDrv_IsThere,
    VC_SampleLoad,
    VC_SampleUnload,
    VC_SampleSpace,
    VC_SampleLength,
    iPhoneDrv_Init,
    iPhoneDrv_Exit,
    NULL,
    VC_SetNumVoices,
    iPhoneDrv_PlayStart,
    iPhoneDrv_PlayStop,
    iPhoneDrv_Update,
    NULL,
    VC_VoiceSetVolume,
    VC_VoiceGetVolume,
    VC_VoiceSetFrequency,
    VC_VoiceGetFrequency,
    VC_VoiceSetPanning,
    VC_VoiceGetPanning,
    VC_VoicePlay,
    VC_VoiceStop,
    VC_VoiceStopped,
    VC_VoiceGetPosition,
    VC_VoiceRealVolume
};


