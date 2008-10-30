/*
 *  RudeControl.h
 *  golf
 *
 *  Created by Robert Rose on 9/15/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __H_RudeControl
#define __H_RudeControl

#include "RudeRect.h"
#include "RudeVertex.h"
#include "RudeTouchTracker.h"

#include <btBulletDynamicsCommon.h>

typedef enum {
	kAnimNone,
	kAnimConstant,
	kAnimPopSlide
} eAnimType;

class RudeControl 
{
public:
	RudeControl();
	
	void SetRect(const RudeRect &r) { m_rect = r; }
	
	bool Contains(const RudeScreenVertex &p);
	
	virtual bool TouchDown(RudeTouch *t);
	virtual bool TouchMove(RudeTouch *t);
	virtual bool TouchUp(RudeTouch *t);
	
	virtual void NextFrame(float delta);
	virtual void Render();
	
	RudeScreenVertex GetDistanceTraveled() { return m_hitDistanceTraveled; }
	RudeScreenVertex GetMoveDelta() { return m_hitMoveDelta; }
	RudeScreenVertex GetHitMove() { return m_hitMove; }
	
	void SetTranslation(const btVector3 &t) { m_translation = t; }
	void SetDesiredTranslation(const btVector3 &desired) { m_desiredTranslation = desired; }
	
	void SetAnimSpeed(float f) { m_animSpeed = f; }
	void SetAnimType(eAnimType at) { m_animType = at; }
	
protected:
	
	RudeRect m_rect;
	RudeScreenVertex m_hitStart;
	RudeScreenVertex m_hitMove;
	RudeScreenVertex m_hitMoveDelta;
	RudeScreenVertex m_hitDelta;
	RudeScreenVertex m_hitDistanceTraveled;
	int m_hitId;
	
	btVector3 m_translation;
	btVector3 m_desiredTranslation;
	float m_animSpeed;
	eAnimType m_animType;
};

#endif
