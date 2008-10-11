/*
 *  RBBallCamera.cpp
 *  golf
 *
 *  Created by Robert Rose on 9/14/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "RBBallCamera.h"

RBBallCamera::RBBallCamera()
: m_ball(0)
, m_terrain(0)
, m_height(0.0f)
, m_desiredHeight(0.0f)
, m_yaw(0.0f)
{
}

void RBBallCamera::Track(eTrackMode mode, btVector3 guide, float height)
{
	m_mode = mode;
	m_guide = guide;
	m_desiredHeight = height;
	
	btVector3 ball = m_ball->GetPosition();
	
	switch(m_mode)
	{
		case kRegardCamera:
			{
				btVector3 forward = m_guide - ball;
				forward.setY(0);
				forward.normalize();
				
				const float kBaseHeight = 25.0f;
				const float kBaseDist = 40.0f;
				
				m_pos = ball - ((kBaseDist) * forward) + btVector3(0,kBaseHeight,0);
			}
			break;
	}
}


void RBBallCamera::NextFrame(float delta)
{
	btVector3 ball = m_ball->GetPosition();
	
	switch(m_mode)
	{
		default:
		case kNone:
			{
				m_pos = btVector3(0,0,-1);
				m_lookAt = btVector3(0,0,0);
			}
			break;
		case kHitCamera:
			{
				m_height += (m_desiredHeight - m_height) * delta * 3.0f;
				
				
				btVector3 forward = m_guide - ball;
				forward.setY(0);
				forward.normalize();
				
				const float kBaseHeight = 12.0f;
				const float kBaseDist = 18.0f;
				
				m_pos = ball - ((m_height + kBaseDist) * forward) + btVector3(0,kBaseHeight + m_height,0);
				m_lookAt = ball + ((m_height + kBaseDist * 2.0f) * forward);
			}
			break;
		case kAimCamera:
			{
				
				m_lookAt += (m_guide - m_lookAt) * delta * 3.0f;
				
				btVector3 offset(0,m_height + 5.0f,18 + m_height);
				
				btMatrix3x3 rot;
				rot.setEulerYPR(m_yaw, 0.0f, 0.0f);
				offset = rot * offset;
				
				btVector3 newpos = m_guide + offset;
				
				m_pos += (newpos - m_pos) * delta * 3.0f;
			
			}
			break;
		case kFixedCamera:
			{
				// slowly lean toward the ball
				
				m_lookAt += (ball - m_lookAt) * delta * 1.0f;
				
				
			}
			break;
		case kFollowCamera:
			{
				m_lookAt = ball;
				
				m_pos = m_lookAt + btVector3(0,20,20);
			}
			break;
		case kRegardCamera:
			{
				btVector3 forward = m_guide - ball;
				float len = forward.length();
				forward.normalize();
				
				
				float halffov = (32.0f / 180.0f) * 3.1415926f;
				
				const float kBaseHeight = 16.0f;
				
				float dist = 20.0f;
				
				forward.setY(0);
				forward.normalize();
				
				btVector3 back = forward * -dist;
				back.setY(kBaseHeight);
				
				m_pos = ball + back;
				//m_pos += (pos - m_pos) * delta * 1.0f;
				
				btVector3 ac = ball - m_pos;
				ac.normalize();
				
				btVector3 zaxis = cross(ac, btVector3(0,1,0));
				
				btQuaternion q(zaxis, halffov);
				
				btVector3 angle = q * ac;
				
				m_lookAt = angle + m_pos;
			}
			break;
	}
}

