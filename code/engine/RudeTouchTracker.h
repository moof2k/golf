/*
 *  RudeTouchTracker.h
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef __H_RBTouchTracker
#define __H_RBTouchTracker

#include "RudeVertex.h"

const int kMaxTouches = 3;

class RudeTouch {

public:
	
	RudeTouch()
	: m_touchId(-1)
	, m_userdata(0)
	{
	}
	
	RudeScreenVertex m_location;
	int m_touchId;
	void *m_userdata;
	
};

class RudeTouchTracker {
	
public:
	RudeTouchTracker();

	RudeTouch * NewTouch(RudeScreenVertex &p);
	RudeTouch * GetTouch(RudeScreenVertex &p);
	void ReleaseTouch(RudeTouch *rbt);
	
	void ReleaseAllTouches();
	
private:
	
	RudeTouch m_touches[kMaxTouches];
	int m_touchCount;
	
};


#endif

