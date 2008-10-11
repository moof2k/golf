/*
 *  RBTouchTracker.h
 *  RudeBlocks
 *
 *  Created by Robert Rose on 8/28/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
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
	
private:
	
	RudeTouch m_touches[kMaxTouches];
	int m_touchCount;
	
};


#endif

