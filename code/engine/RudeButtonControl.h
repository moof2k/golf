/*
 *  RudeButtonControl.h
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef __H_RudeButtonControl
#define __H_RudeButtonControl

#include "RudeControl.h"

class RudeButtonControl : public RudeControl
{
public:
	
	RudeButtonControl(RudeControl *parent);
	
	void SetTexture(const char *texturename, int offx = -1, int offy = -1);
	
	virtual void OnReposition();
	
	virtual bool TouchDown(RudeTouch *t);
	virtual bool TouchMove(RudeTouch *t);
	virtual bool TouchUp(RudeTouch *t);
	
	virtual void Render();
	
protected:
	
	float m_points[8];
	float m_uvs[8];
	
	bool m_state;
	int m_texid;
	
	int m_texsize;
	int m_offx;
	int m_offy;
};

#endif
