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
	
	RudeButtonControl();
	
	void SetTextures(const char *stateoff, const char *stateon);
	
	virtual void SetRect(const RudeRect &r);
	
	virtual bool TouchDown(RudeTouch *t);
	virtual bool TouchMove(RudeTouch *t);
	virtual bool TouchUp(RudeTouch *t);
	
	virtual void Render();
	
protected:
	
	float m_points[8];
	
	bool m_state;
	int m_stateofftex;
	int m_stateontex;
	
	int m_texsize;
	int m_halfTexsize;
	
};

#endif
