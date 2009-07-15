/*
 *  RudeVertex.h
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef _H_RudeVertex
#define _H_RudeVertex

class RudeScreenVertex {
	
public:
	RudeScreenVertex()
	: m_x(0), m_y(0)
	{
	}
	
	RudeScreenVertex(int x, int y)
	: m_x(x), m_y(y)
	{
	}
	
	int m_x, m_y;
	
	bool operator==(const RudeScreenVertex &other)
	{
		if((other.m_x == m_x) && (other.m_y == m_y))
		   return true;
		else
		   return false;
	}
	
	RudeScreenVertex & operator+=(const RudeScreenVertex &other)
	{
		m_x += other.m_x;
		m_y += other.m_y;
		
		return *this;
	}
	
	RudeScreenVertex operator-(const RudeScreenVertex &other) const
	{
		RudeScreenVertex p(m_x - other.m_x, m_y - other.m_y);
		return p;
	}
};


#endif