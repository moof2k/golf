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


class RudeColorInt {

public:
	RudeColorInt();
	RudeColorInt(int r, int g, int b);

	void SetInt(int r, int g, int b)
	{
		m_r = r;
		m_g = g;
		m_b = b;
	}

	void SetFloat(float r, float g, float b)
	{
		m_r = (int) (r * 255.0f);
		m_g = (int) (g * 255.0f);
		m_b = (int) (b * 255.0f);
	}

	int m_r;
	int m_g;
	int m_b;

private:

};

class RudeColorFloat {

public:
	RudeColorFloat();
	RudeColorFloat(float r, float g, float b);

	RudeColorFloat(const RudeColorInt &intcolor)
	{
		SetInt(intcolor.m_r, intcolor.m_g, intcolor.m_b);
	}
	
	void SetFloat(float r, float g, float b)
	{
		m_r = r;
		m_g = g;
		m_b = b;
	}

	void SetInt(int r, int g, int b)
	{
		m_r = ((float) r) / 255.0f;
		m_g = ((float) g) / 255.0f;
		m_b = ((float) b) / 255.0f;
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