
//=============================================================================
//== Windows Mobile Sound Engine
//== Digital Concepts (c) 2003 All Rights Reserved
//== Not for use or distribution
//=============================================================================

#ifndef _SOUND_H
#define _SOUND_H

//== Start-up & Shutdown ======================================================

extern bool   soundInit(void);             //== Initialize Sound Engine
extern bool   soundShutdown(void);         //== De-Initialize Sound Engine

//== Load & Unload ============================================================

extern void   loadSound(char *filename,int num); //== Use num values 0->40
extern void   loadSound(char *filename,int num,int loopStart,int loopEnd);

extern void   unloadSound(int num);        //== Remove a sound from memory
extern void   makeSoundLoop(int num);      //== Set a sound to loop
extern void   clearSoundLoop(int num);     //== Unloop a sound

//== Playing Sounds ===========================================================

extern void   playSound(int num);          //== Play a sound
extern void   stopSound(int num);          //== Stop playing a sound
extern int    noSound(int num);            //== Check if a sound is not playing
extern void   stopAllSound(void);          //== Silence all sounds

//== Playing Music ============================================================

extern bool   playSong(char *name);        //== Play a song by filename
                                           //== Automatically loads song
extern void   playSongPattern(int pos);    //== Jump to specific pattern


//== Volume Control ===========================================================

extern void   setSFXVolume(int volume);    //== Set SFX   Volume 0->255
extern void   setMusicVolume(int volume);  //== Set Music Volume 0->255
extern int    getSFXVolume(void);          //== Get SFX   Volume 0->255
extern int    getMusicVolume(void);        //== Get Music Volume 0->255

//== Sound Buffer Updates =====================================================

extern void updateSoundBuffers(void);  //== Call this often in your main loop

//== Sound Quality Params =====================================================

                           // Change these values prior to calling soundInit()

extern int soundQuality;   // 1=generic 8 bit output
                           // 2=generic 8 bit output + interpolation
                           // 3=16 bit output + interp + high quality mixing
                           //
                           // default=3;

extern int soundFreq;      // 1=11Khz 2=22Khz 3=44Khz    default=2
extern int soundStereo;    // 0=mono 1=stereo            default=1
	
//== That's all there is to it ================================================

#endif