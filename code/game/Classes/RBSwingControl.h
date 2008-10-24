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
#include "RudeTextControl.h"
#include "RBGolferObject.h"

#include <unistd.h>
#include <mach/mach.h>
#include <mach/mach_time.h>

const int kMaxSwingPoints = 256;

typedef enum {
	kNoStroke,
	kBeginStroke,
	kDownStroke,
	kUpStroke,
	kStrokeComplete
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
	
	void Reset();
	
	void NextFrame(float delta);
	
	void SetGolfer(RBGolferObject *golfer) { m_golfer = golfer; }
	
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
	
	void RenderPower();
	void RenderRing();
	void RenderTracks();
	
	void AddSwingPoint(const RudeScreenVertex &p, bool first);
	
	RBGolferObject *m_golfer;
	
	RBSwingPoint m_swingPoints[kMaxSwingPoints];
	RudeTextControl m_swingPowerText;
	
	int m_curSwingPoint;
	
	uint64_t m_firstTime;
	
	float m_downTimer;
	float m_downOptimalPct;
	
	float m_downBonusPower;
	float m_downBasePower;
	
	
	float m_downTime;
	float m_upTime;
	
	float m_upStrokeDeviation;
	
	RudeScreenVertex m_lastPoint;
	RudeScreenVertex m_downStroke;
	RudeScreenVertex m_upStroke;
	
	eStrokeState m_strokeState;
	
	int m_ringTextureId;
	
	bool m_backStrokeAnimDone;
	bool m_fwdStrokeAnimSent;
	
};

#endif

