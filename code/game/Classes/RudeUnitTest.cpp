/*
 *  RudeUnitTest.cpp
 *  golf
 *
 *  Created by Robert Rose on 3/12/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
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
