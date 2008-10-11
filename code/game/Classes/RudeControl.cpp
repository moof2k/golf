/*
 *  RudeControl.cpp
 *  golf
 *
 *  Created by Robert Rose on 9/15/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "RudeControl.h"

RudeControl::RudeControl()
: m_rect(0,0,0,0)
, m_hitId(-1)
{
}

bool RudeControl::Contains(const RudeScreenVertex &p)
{
	return m_rect.Contains(p);
}

bool RudeControl::TouchDown(RudeTouch *t)
{
	if(!Contains(t->m_location))
		return false;
	
	m_hitId = t->m_touchId;
	m_hitStart = t->m_location;
	m_hitMove = m_hitStart;
	m_hitMoveDelta = RudeScreenVertex(0,0);
	m_hitDelta = RudeScreenVertex(0,0);
	m_hitDistanceTraveled = RudeScreenVertex(0,0);
	
	return true;
}

bool RudeControl::TouchMove(RudeTouch *t)
{
	if(m_hitId != t->m_touchId)
		return false;
	
	m_hitMoveDelta = t->m_location - m_hitMove;
	m_hitMove = t->m_location;
	m_hitDelta = t->m_location - m_hitStart;
	
	m_hitDistanceTraveled.m_x += abs(m_hitDelta.m_x);
	m_hitDistanceTraveled.m_y += abs(m_hitDelta.m_y);
	
	return true;
}

bool RudeControl::TouchUp(RudeTouch *t)
{
	if(m_hitId != t->m_touchId) 
		return false;
	
	m_hitId = -1;
	
	m_hitMoveDelta = t->m_location - m_hitMove;
	m_hitMove = t->m_location;
	m_hitDelta = t->m_location - m_hitStart;
	
	return true;
}


