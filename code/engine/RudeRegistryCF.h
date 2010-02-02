/*
 *  RudeRegistryCF.h
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef _H_RudeRegistryCF
#define _H_RudeRegistryCF

#include "RudeRegistry.h"

/**
 * Registry implementation for MacOS and iPhone
 */
class RudeRegistryCF : public RudeRegistry
{
public:
	RudeRegistryCF(void);
	~RudeRegistryCF(void);

	int QueryByte(const TCHAR *app, const TCHAR *name, void *buffer, int *buffersize);
	int SetByte(const TCHAR *app, const TCHAR *name, void *buffer, int buffersize);

private:


};

#endif