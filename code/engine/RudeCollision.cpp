/*
 *  RudeCollision.cpp
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#include "RudeCollision.h"

#include "Rude.h"


// http://local.wasp.uwa.edu.au/~pbourke/geometry/sphereline/
//
// (x3 - x1)(x2 - x1) + (y3 - y1)(y2 - y1) + (z3 - z1)(z2 - z1) 
// -----------------------------------------------------------   = u
// (x2 - x1)(x2 - x1) + (y2 - y1)(y2 - y1) + (z2 - z1)(z2 - z1)
//
// if 0 <= u <= 1 then intersection
// point = p1 + (p2 - p1) * u
//
// Returns the number of intersections

int RudeCollision::LineSegmentAndSphere(const btVector3 &p1, const btVector3 &p2, const btVector3 &sc, float r, btVector3 &int_p1, btVector3 &int_p2)
{
	const float kEps = 0.00001f;
	
	btVector3 dp = p2 - p1;
	
	float a = dp.x() * dp.x() + dp.y() * dp.y() + dp.z() * dp.z();
	float b = 2.0f * (dp.x() * (p1.x() - sc.x()) + dp.y() * (p1.y() - sc.y()) + dp.z() * (p1.z() - sc.z()));
	float c = sc.x() * sc.x() + sc.y() * sc.y() + sc.z() * sc.z();
	
	c += p1.x() * p1.x() + p1.y() * p1.y() + p1.z() * p1.z();
	c -= 2 * (sc.x() * p1.x() + sc.y() * p1.y() + sc.z() * p1.z());
	c -= r * r;
	
	float bb4ac = b * b - 4 * a * c;
	float absa = a > 0.0f ? a : -a;
	if (absa < kEps || bb4ac < 0.0f)
		return 0;
	
	float abs_bb4ac = bb4ac ? bb4ac : -bb4ac;
	if(abs_bb4ac < kEps)
	{
		float u = -b / (2.0f * a);
		int_p1 = p1 + u * dp;
		int_p2 = int_p1;
		return 1;
	}
	
	int solns = 0;
	
	float bb4acsqrt = sqrt(bb4ac);
	float a2 = 2.0f * a;
	
	float u1 = (-b + bb4acsqrt) / a2;
	
	if(0.0f <= u1 && u1 <= 1.0f)
	{
		int_p1 = p1 + u1 * dp;
		int_p2 = int_p1;
		solns++;
	}
	
	float u2 = (-b - bb4acsqrt) / a2;
	
	if(0.0f <= u2 && u2 <= 1.0f)
	{
		if(solns)
		{
			int_p2 = p1 + u2 * dp;
		}
		else
		{
			int_p1 = p1 + u2 * dp;
			int_p2 = int_p2;
		}
		solns++;
	}
	
	return solns;
}


void RudeCollision::UnitTest()
{
	{
		btVector3 p1(0,0,0);
		btVector3 p2(1,0,0);
		btVector3 sc(0,0,0);
		float radius = 0.5;
		btVector3 int_p1, int_p2;
		
		int result = RudeCollision::LineSegmentAndSphere(p1, p2, sc, radius, int_p1, int_p2);
		RUDE_ASSERT(result == 1, "Fail");
		RUDE_ASSERT(int_p1.x() == 0.5f && int_p1.y() == 0.0f && int_p1.z() == 0.0f, "Fail");
	}
	
	{
		btVector3 p1(0,0,0);
		btVector3 p2(2,0,0);
		btVector3 sc(1,0,0);
		float radius = 0.5;
		btVector3 int_p1, int_p2;
		
		int result = RudeCollision::LineSegmentAndSphere(p1, p2, sc, radius, int_p1, int_p2);
		RUDE_ASSERT(result == 2, "Fail");
		RUDE_ASSERT(int_p1.x() == 1.5f && int_p1.y() == 0.0f && int_p1.z() == 0.0f, "Fail");
		RUDE_ASSERT(int_p2.x() == 0.5f && int_p2.y() == 0.0f && int_p2.z() == 0.0f, "Fail");
	}
	
	{
		btVector3 p1(0,0,0);
		btVector3 p2(0,0,2);
		btVector3 sc(0,0,0);
		float radius = 1.0f;
		btVector3 int_p1, int_p2;
		
		int result = RudeCollision::LineSegmentAndSphere(p1, p2, sc, radius, int_p1, int_p2);
		RUDE_ASSERT(result == 1, "Fail");
		RUDE_ASSERT(int_p1.x() == 0.0f && int_p1.y() == 0.0f && int_p1.z() == 1.0f, "Fail");
	}
	
	{
		btVector3 p1(100,100,100);
		btVector3 p2(200,100,100);
		btVector3 sc(150,100,100);
		float radius = 25.0f;
		btVector3 int_p1, int_p2;
		
		int result = RudeCollision::LineSegmentAndSphere(p1, p2, sc, radius, int_p1, int_p2);
		RUDE_ASSERT(result == 2, "Fail");
		RUDE_ASSERT(int_p1.x() == 175.0f && int_p1.y() == 100.0f && int_p1.z() == 100.0f, "Fail");
	}
	
	{
		// Really big circle contains line segment
		btVector3 p1(100,100,100);
		btVector3 p2(200,100,100);
		btVector3 sc(150,100,100);
		float radius = 100.0f;
		btVector3 int_p1, int_p2;
		
		int result = RudeCollision::LineSegmentAndSphere(p1, p2, sc, radius, int_p1, int_p2);
		RUDE_ASSERT(result == 0, "Fail");
	}
	
	{
		// No intersections at all
		btVector3 p1(100,100,100);
		btVector3 p2(200,100,100);
		btVector3 sc(150,1000,100);
		float radius = 100.0f;
		btVector3 int_p1, int_p2;
		
		int result = RudeCollision::LineSegmentAndSphere(p1, p2, sc, radius, int_p1, int_p2);
		RUDE_ASSERT(result == 0, "Fail");
	}
}

