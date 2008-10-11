/*
 *  RBSwingControl.h
 *  golf
 *
 *  Created by Robert Rose on 9/16/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __H_RBSwingControl
#define __H_RBSwingControl

#include "RudeControl.h"
#include "RudeVertex.h"

#include <unistd.h>
#include <mach/mach.h>
#include <mach/mach_time.h>

const int kMaxSwingPoints = 256;

typedef enum {
	kDownStroke,
	kUpStroke
} eStrokeState;

class RBSwingPoint {
public:
	
	RBSwingPoint()
	: m_p(0,0)
	, m_time(0)
	{
	}
	
	RBSwingPoint(const RudeScreenVertex &p, float time, eStrokeState state)
	: m_p(p)
	, m_time(time)
	, m_state(state)
	{
	}
	

	RudeScreenVertex m_p;
	float m_time;
	eStrokeState m_state;
	
};

class RBSwingControl : public RudeControl {
	
public:
	RBSwingControl();
	
	void SetTexture(const char *texname);
	
	void Reset();
	
	virtual bool TouchDown(RudeTouch *t);
	virtual bool TouchMove(RudeTouch *t);
	virtual bool TouchUp(RudeTouch *t);
	
	virtual void Render();
	
	void GetStroke(RudeScreenVertex &downStroke, float &downTime, RudeScreenVertex &upStroke, float &upTime)
	{
		downStroke = m_downStroke;
		downTime = m_downTime;
		upStroke = m_upStroke;
		upTime = m_upTime;
	}
	
	float GetPower();
	float GetAngle();
	float GetImpact();
	
	bool CanSwing();
	bool WillSwing();
	
	
private:
	
	void AddSwingPoint(const RudeScreenVertex &p, bool first);
	
	RBSwingPoint m_swingPoints[kMaxSwingPoints];
	int m_curSwingPoint;
	
	uint64_t m_firstTime;
	
	float m_downTime;
	float m_upTime;
	
	RudeScreenVertex m_lastPoint;
	RudeScreenVertex m_downStroke;
	RudeScreenVertex m_upStroke;
	
	eStrokeState m_strokeState;
	
	int m_textureId;
	int m_textureSize;
	
};

#endif

