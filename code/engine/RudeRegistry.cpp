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

#if defined(RUDE_IPHONE) || defined(RUDE_MACOS)
#include "RudeRegistryCF.h"
#endif

RudeRegistry::RudeRegistry(void)
{
}

RudeRegistry::~RudeRegistry(void)
{
}

/**
 * Get a pointer to the RudeRegistry singleton for the currently running platform.
 * 
 * iPhone/iPod/MacOS: RudeRegistryCF\n
 * Windows/PocketPC: RudeRegistryWin\n
 * Symbian: RudeRegistrySymbian
 */
RudeRegistry * RudeRegistry::GetSingleton()
{
	
#if defined(RUDE_IPHONE) || defined(RUDE_MACOS)
	
	static RudeRegistryCF *reg = 0;
	
	if(reg == 0)
		reg = new RudeRegistryCF();
	
	return (RudeRegistry *) reg;

#elif defined(RUDE_WIN)

	static RudeRegistryWin *reg = 0;

	if(reg == 0)
		reg = new RudeRegistryWin();

	return (RudeRegistry *) reg;
	
#else
	return RUDE_ASSERT(0, "RudeRegistry not defined");
#endif
}