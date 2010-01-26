/*
 *  RudeRegistryText.h
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef _H_RudeRegistryText
#define _H_RudeRegistryText

#include "RudeRegistry.h"

/**
 * Registry implementation that uses text files.  This class is mostly only useful for debugging.
 * Look at one of the platform-specific implementation for routine use.
 */
class RudeRegistryText : public RudeRegistry
{
public:
	RudeRegistryText(void);
	~RudeRegistryText(void);

	long QueryByte(TCHAR *app, TCHAR *name, void *buffer, long *buffersize);
	long SetByte(TCHAR *app, TCHAR *name, void *buffer, long buffersize);

private:


};

#endif