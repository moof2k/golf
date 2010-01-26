/*
 *  RudeFile.cpp
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#include "RudeFile.h"

#include <CoreFoundation/CFBundle.h>

/**
 * Returns the full location of a file in a cross-platform compatible manner.
 */
bool RudeFileGetFile(const char *filename, char *buffer, int bufsize, bool canfail)
{
	
	CFStringRef cfFilename = CFStringCreateWithCString(0, filename, kCFStringEncodingASCII);
	CFBundleRef mainBundle = CFBundleGetMainBundle();
	CFURLRef url = CFBundleCopyResourceURL(mainBundle, cfFilename, 0, 0);
	
	if(url == NULL)
	{
		buffer[0] = '\0';
		RUDE_ASSERT(canfail, "Could not locate file %s", filename);
		return false;
	}
	
	CFURLGetFileSystemRepresentation(url, true, (UInt8 *) buffer, bufsize);
	
	CFRelease(url);
	CFRelease(cfFilename);
	
	return true;
}


