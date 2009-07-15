/*
 *  RudeFile.h
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */


#ifndef __H_RudeFile
#define __H_RudeFile

#include "Rude.h"

bool RudeFileGetFile(const char *filename, char *buffer, int bufsize, bool canfail = false);

#endif 

