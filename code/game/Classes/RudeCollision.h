/*
 *  RudeCollision.h
 *  golf
 *
 *  Created by Robert Rose on 3/11/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __H_RudeCollision
#define __H_RudeCollision

#include <btBulletDynamicsCommon.h>

class RudeCollision
{
public:
	
	static int LineSegmentAndSphere(const btVector3 &line_p1, const btVector3 &line_p2, const btVector3 &circle_p, float radius, btVector3 &int_p1, btVector3 &int_p2);
	
	static void UnitTest();
	
};

#endif // __H_RudeCollision