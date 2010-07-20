/*
 *  RudeTouchTracker.cpp
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#include "RudeTouchTracker.h"


RudeTouchTracker::RudeTouchTracker()
: m_touchCount(0)
{
	
}

RudeTouch * RudeTouchTracker::NewTouch(RudeScreenVertex &p)
{
	for(int i = 0; i < kMaxTouches; i++)
	{
		if(m_touches[i].m_touchId < 0)
		{
			m_touches[i].m_location = p;
			m_touches[i].m_touchId = m_touchCount++;
			return &m_touches[i];
		}
	}
	
	return 0;
}

RudeTouch * RudeTouchTracker::GetTouch(RudeScreenVertex &p)
{
	for(int i = 0; i < kMaxTouches; i++)
	{
		RudeTouch *t = &m_touches[i];
		if(t->m_location.m_x == p.m_x)
			if(t->m_location.m_y == p.m_y)
				return t;
	}
	
	return 0;
}

void RudeTouchTracker::ReleaseTouch(RudeTouch *rbt)
{
	for(int i = 0; i < kMaxTouches; i++)
	{
		if(rbt->m_touchId == m_touches[i].m_touchId)
		{
			m_touches[i].m_touchId = -1;
			m_touches[i].m_userdata = 0;
		}
	}
}

void RudeTouchTracker::ReleaseAllTouches()
{
	for(int i = 0; i < kMaxTouches; i++)
	{
		if(m_touches[i].m_touchId >= 0)
		{
			m_touches[i].m_touchId = -1;
			m_touches[i].m_userdata = 0;
		}
	}
}


