#ifndef _H_RudeVertex
#define _H_RudeVertex

#include "RudeColor.h"

typedef struct RudeVertex {
	float x, y, z;
	RudeColor c;
} RudeVertex;

typedef struct RudeVertexT {
	float x, y, z;
	float s, t;
} RudeVertexT;

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