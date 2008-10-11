/*
 *  RudeFile.cpp
 *  golf
 *
 *  Created by Robert Rose on 9/4/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "RudeFile.h"

#include <CoreFoundation/CFBundle.h>

void RudeFileGetFile(const char *filename, char *buffer, int bufsize)
{
	
	CFStringRef cfFilename = CFStringCreateWithCString(0, filename, kCFStringEncodingASCII);
	CFBundleRef mainBundle = CFBundleGetMainBundle();
	CFURLRef url = CFBundleCopyResourceURL(mainBundle, cfFilename, 0, 0);
	
	if(url == NULL)
	{
		buffer[0] = '\0';
		RUDE_ASSERT(0, "Could not locate file %s", filename);
		return;
	}
	
	CFURLGetFileSystemRepresentation(url, true, (UInt8 *) buffer, bufsize);
	
	CFRelease(url);
	CFRelease(cfFilename);
}


