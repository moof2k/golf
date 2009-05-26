/*
 *  RBTButton.cpp
 *  RudeBlocks
 *
 *  Created by Robert Rose on 8/27/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "RBTButton.h"

#include "RudeTextureManager.h"

RBTButton::RBTButton()
: _tid(-1)
, _touch(0)
, _x(0.0f)
, _y(0.0f)
, _w(1.0f)
, _h(1.0f)
, _fliphorz(false)
{
	
}


RBTButton::~RBTButton()
{
	
}


void RBTButton::SetTexture(const char *name)
{
	_tid = RudeTextureManager::GetInstance()->LoadTextureFromPNGFile(name);
	if(_tid < 0)
		return;
	
	_w = RudeTextureManager::GetInstance()->GetTexture(_tid)->GetWidth();
	_h = RudeTextureManager::GetInstance()->GetTexture(_tid)->GetHeight();
}

void RBTButton::Render(Rude3DSurface *backbuffer3d)
{
	RudeTextureManager::GetInstance()->SetTexture(_tid);
	
	float x = _x;
	float y = _y;
	float w = _w;
	float h = _h;
	
	float s = 1.0f;
	
	if(_fliphorz)
	{
		s = -1.0f;
	}
	
	RudeVertexT p[4];
	
	p[0].x = x;
	p[0].y = y;
	p[0].z = 0.0f;
	p[0].s = 0.0f;
	p[0].t = 1.0f;
	
	p[1].x = x + w - 1.0f;
	p[1].y = y;
	p[1].z = 0.0f;
	p[1].s = s;
	p[1].t = 1.0f;
	
	p[2].x = x;
	p[2].y = y + h - 1.0f;
	p[2].z = 0.0f;
	p[2].s = 0.0f;
	p[2].t = 0.0f;
	
	p[3].x = x + w - 1.0f;
	p[3].y = y + h - 1.0f;
	p[3].z = 0.0f;
	p[3].s = s;
	p[3].t = 0.0f;
	
	backbuffer3d->GrlDrawTriStripT(p, 4);
}

bool RBTButton::Hit(RudeScreenVertex &p)
{
	float x = (480.0f - p.y) - 240.0f;
	float y = (320.0f - p.x) - 160.0f;
	
	if(x >= _x && x <= (_x + _w))
		if(y >= _y && y <= (_y + _h))
			return true;
	
	return false;
}


