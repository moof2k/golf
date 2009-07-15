/*
 *  MikSound.cpp
 *  VectorBlaster
 *
 *  Created by Robert Rose on 8/20/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */


#include "Rude.h"
#include "MikSound.h"

#include <AudioToolbox/AudioQueue.h>

//typedef bool BOOL;
#include "mikmod.h"

extern MDRIVER iPhoneDrv;
extern AudioQueueRef mMikModAudioQueue;

MODULE *module = 0;

void MikSound_Init()
{
	MikMod_RegisterDriver( &iPhoneDrv );
    MikMod_RegisterAllLoaders();
	
	char *initStr = "buffers=2,size=20000,volume=%.5f";
	MikMod_Init(initStr);
}

void MikSound_PlayModule(char *filename)
{
	if(module)
	{
		Player_Stop();
		Player_Free(module);
		module = 0;
	}
	
	CFBundleRef bundle = CFBundleGetMainBundle();
	CFStringRef file = CFStringCreateWithCString(0, filename, kCFStringEncodingASCII);
	CFURLRef url = CFBundleCopyResourceURL(bundle, file, 0, 0);
	CFStringRef path = CFURLCopyFileSystemPath(url, kCFURLPOSIXPathStyle);
	
	char thepath[512];
	CFStringGetCString(path, thepath, 512, kCFStringEncodingASCII);
	
	module = Player_Load(thepath, 64, 0);
	
	if(module)
		Player_Start(module);
}

