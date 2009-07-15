/*
 *  RudeRect.h
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef __H_RudeRect
#define __H_RudeRect

#include "RudeVertex.h"

class RudeRect {

public:
	
	RudeRect() 
	: m_top(0)
	, m_left(0)
	, m_bottom(0)
	, m_right(0)
	{
	}
	
	RudeRect(int top, int left, int bottom, int right)
	: m_top(top)
	, m_left(left)
	, m_bottom(bottom)
	, m_right(right)
	{
	}
	
	bool Contains(const RudeScreenVertex &p)
	{
		if(p.m_x >= m_left && p.m_x <= m_right)
			if(p.m_y <= m_bottom && p.m_y >= m_top)
				return true;
		
		return false;
	}
	
	int m_top;
	int m_left;
	int m_bottom;
	int m_right;


private:

};

#endif
