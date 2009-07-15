/*
 *  RudeRegistry.cpp
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#include "ruderegistry.h"

#ifdef RUDE_WIN
#include "RudeRegistryWin.h"
#include "RudeRegistryText.h"
#endif

#ifdef RUDE_SYMBIAN
#include "RudeRegistrySymbian.h"
#include "RudeRegistryText.h"
#endif

#ifdef RUDE_IPHONE
#include "RudeRegistryCF.h"
#endif

RudeRegistry::RudeRegistry(void)
{
}

RudeRegistry::~RudeRegistry(void)
{
}

RudeRegistry * RudeRegistry::GetSingleton()
{
	
#ifdef RUDE_IPHONE
	static RudeRegistryCF *reg = 0;
	
	if(reg == 0)
		reg = new RudeRegistryCF();
	
	return (RudeRegistry *) reg;
#endif


	return NULL;
}