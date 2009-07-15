/*
 *  RudeRegistry.h
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef _H_RudeRegistry
#define _H_RudeRegistry

#include "RudeGlobals.h"

class RudeRegistry
{
public:
	RudeRegistry(void);
	virtual ~RudeRegistry(void);

	static RudeRegistry * GetSingleton();

	virtual int QueryByte(const TCHAR *app, const TCHAR *name, void *buffer, int *buffersize) = 0;
	virtual int SetByte(const TCHAR *app, const TCHAR *name, void *buffer, int buffersize) = 0;

};

#endif