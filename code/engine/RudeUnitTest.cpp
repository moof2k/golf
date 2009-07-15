/*
 *  RudeUnitTest.cpp
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#include "RudeUnitTest.h"


#define RUDE_UNIT_TEST



#ifdef RUDE_UNIT_TEST

#include "RudeCollision.h"

void RudeUnitTest::UnitTest()
{
	RudeCollision::UnitTest();
}

#else

void RudeUnitTest::UnitTest()
{
	
}

#endif
