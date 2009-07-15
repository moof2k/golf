/*
 *  RudeColor.h
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef __H_RudeColor
#define __H_RudeColor

#include "Rude.h"

class RudeColorFloat {

public:
	RudeColorFloat();
	RudeColorFloat(float r, float g, float b);
	
	void Set(float r, float g, float b)
	{
		m_r = r;
		m_g = g;
		m_b = b;
	}

	void Blend(const RudeColorFloat &other, float scale)
	{
		float invscale = 1.0f - scale;
		m_r = invscale * m_r + scale * other.m_r;
		m_g = invscale * m_g + scale * other.m_g;
		m_b = invscale * m_b + scale * other.m_b;
	}
	
	RudeColorFloat & operator*=(float scale)
	{
		RUDE_ASSERT(scale >= -1.0f, "Invalid scale");
		RUDE_ASSERT(scale <= 1.0f, "Invalid scale");
		
		m_r *= scale;
		m_g *= scale;
		m_b *= scale;
		
		return *this;
	}
	
	float m_r;
	float m_g;
	float m_b;
	
private:

};


#endif