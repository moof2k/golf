/*
 *  RBDebugCamera.h
 *  golf
 *
 *  Created by Robert Rose on 9/7/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __H_RBDebugCamera
#define __H_RBDebugCamera


#include "Rude.h"
#include "RudeTouchTracker.h"
#include "RBCamera.h"


class RBDebugCamera : public RBCamera
{
	
public:
	RBDebugCamera();
	
	void NextFrame(float delta);
	
	void TouchDown(RudeTouch *rbt);
	void TouchMove(RudeTouch *rbt);
	void TouchUp(RudeTouch *rbt);
	
private:
	
	btVector3 m_ypr;
	
	btVector3 m_right;
	btVector3 m_forward;
	
	btVector3 m_movePulse;
	btVector3 m_rotPulse;
	
	int m_touchCount;
	
	int m_touchMoveID;
	RudeScreenVertex m_prevMove;
	
	int m_touchRotID;
	RudeScreenVertex m_prevRot;
	
};

#endif

