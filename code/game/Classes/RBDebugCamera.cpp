/*
 *  RBDebugCamera.cpp
 *  golf
 *
 *  Created by Robert Rose on 9/7/08.
 *  Copyright 2008 Bork 3D LLC. All rights reserved.
 *
 */

#include "RBDebugCamera.h"
#include "RudeGL.h"
#include "btMatrix3x3.h"
#include "btTransform.h"


RBDebugCamera::RBDebugCamera()
: m_ypr(0,0,0)
, m_right(1,0,0)
, m_forward(0,0,1)
, m_touchCount(0)
, m_movePulse(0,0,0)
, m_rotPulse(0,0,0)
, m_touchMoveID(-1)
, m_touchRotID(-1)
{

}

void RBDebugCamera::NextFrame(float delta)
{
	const float kMoveSlideOffDamp = 0.5f * delta;
	const float kRotSlideOffDamp = 0.999f * delta;
	
	if(m_touchMoveID < 0)
	{
		m_movePulse -= m_movePulse * kMoveSlideOffDamp;
	}
	if(m_touchRotID < 0)
	{
		m_rotPulse -= m_rotPulse * kRotSlideOffDamp;
	}
	
	
	m_ypr += m_rotPulse;
	
	//m_yaw += 0.1f * delta;
	//m_pitch += 0.1f * delta;
	btMatrix3x3 mat;
	mat.setEulerYPR(m_ypr.x(), m_ypr.y(), m_ypr.z());
	btTransform trans(mat);
	
	m_pos += trans * m_movePulse;
	
	btVector3 z(0,0,1);
	btVector3 x(1,0,0);
	
	m_forward = trans * z;
	m_right = trans * x;

	m_up = cross(m_forward, m_right);
	m_lookAt = m_pos + m_forward;
	
}



void RBDebugCamera::TouchDown(RudeTouch *rbt)
{
	if(rbt->m_location.m_y < 240)
		return;
	
	if(rbt->m_location.m_x < 160)
	{
		m_touchMoveID = rbt->m_touchId;
		m_prevMove = rbt->m_location;
		m_movePulse = btVector3(0,0,0);
	}
	else
	{
		m_touchRotID = rbt->m_touchId;
		m_prevRot = rbt->m_location;
		m_rotPulse = btVector3(0,0,0);
	}
}

void RBDebugCamera::TouchMove(RudeTouch *rbt)
{
	const float kMoveDamp = 0.01f;
	const float kRotDamp = 0.0001f;
	
	if(rbt->m_touchId == m_touchMoveID)
	{
		int dx = rbt->m_location.m_x - m_prevMove.m_x;
		int dy = rbt->m_location.m_y - m_prevMove.m_y;
		
		float movex = float(-dx) * kMoveDamp;
		float movez = float(-dy) * kMoveDamp;
		
		m_movePulse = btVector3(movex,0,movez);
		
	}
	else if(rbt->m_touchId == m_touchRotID)
	{
		int dx = rbt->m_location.m_x - m_prevRot.m_x;
		int dy = rbt->m_location.m_y - m_prevRot.m_y;
		
		float roty = float(dx) * kRotDamp;
		float rotp = float(dy) * kRotDamp;
		
		m_rotPulse = btVector3(roty,rotp,0);
		
	}
}

void RBDebugCamera::TouchUp(RudeTouch *rbt)
{
	if(rbt->m_touchId == m_touchMoveID)
	{
		m_touchMoveID = -1;
	}
	else if(rbt->m_touchId == m_touchRotID)
	{
		m_touchRotID = -1;
	}
}


