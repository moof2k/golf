/*
 *  RudeControl.h
 *  golf
 *
 *  Created by Robert Rose on 9/15/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __H_RudeControl
#define __H_RudeControl

#include "RudeRect.h"
#include "RudeVertex.h"
#include "RudeTouchTracker.h"

class RudeControl 
{
public:
	RudeControl();
	
	void SetRect(const RudeRect &r) { m_rect = r; }
	
	bool Contains(const RudeScreenVertex &p);
	
	virtual bool TouchDown(RudeTouch *t);
	virtual bool TouchMove(RudeTouch *t);
	virtual bool TouchUp(RudeTouch *t);
	
	virtual void Render() {}
	
	RudeScreenVertex GetDistanceTraveled() { return m_hitDistanceTraveled; }
	RudeScreenVertex GetMoveDelta() { return m_hitMoveDelta; }
	
protected:
	
	RudeRect m_rect;
	RudeScreenVertex m_hitStart;
	RudeScreenVertex m_hitMove;
	RudeScreenVertex m_hitMoveDelta;
	RudeScreenVertex m_hitDelta;
	RudeScreenVertex m_hitDistanceTraveled;
	int m_hitId;
};

#endif
