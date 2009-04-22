/*
 *  RudeFile.cpp
 *  golf
 *
 *  Created by Robert Rose on 9/4/08.
 *  Copyright 2008 Bork 3D LLC. All rights reserved.
 *
 */

#include "RudeFile.h"

#include <CoreFoundation/CFBundle.h>

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


