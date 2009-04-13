/*
 *  RBBallCamera.cpp
 *  golf
 *
 *  Created by Robert Rose on 9/14/08.
 *  Copyright 2008 Bork 3D LLC. All rights reserved.
 *
 */

#include "RBBallCamera.h"

RBBallCamera::RBBallCamera()
: m_ball(0)
, m_terrain(0)
, m_height(0.0f)
, m_guide(0.0f, 0.0f, 0.0f)
, m_desiredGuide(0.0f, 0.0f, 0.0f)
, m_desiredHeight(0.0f)
, m_yaw(0.0f)
, m_smooth(false)
{
}

void RBBallCamera::SetTrackMode(eTrackMode mode)
{
	eTrackMode prevmode = m_mode;
	m_mode = mode;
	
	btVector3 ball = m_ball->GetPosition();
	
	m_halffov = (16.0f / 180.0f) * 3.1415926f;
	
	switch(m_mode)
	{
		case kHitCamera:
			{
				if(prevmode == kPuttCamera)
					m_smooth = true;
				else
					m_smooth = false;
			}
			break;
		case kPuttCamera:
			{
				if(prevmode == kHitCamera)
					m_smooth = true;
				else
					m_smooth = false;
			}
			break;
		case kRegardCamera:
			{
				btVector3 forward = m_desiredGuide - ball;
				forward.setY(0);
				forward.normalize();
				
				const float kBaseHeight = 25.0f;
				const float kBaseDist = 40.0f;
				
				m_pos = ball - ((kBaseDist) * forward) + btVector3(0,kBaseHeight,0);
			}
			break;
		case kPlacementCamera:
			m_lookAt = ball;
			break;
	}
}


void RBBallCamera::NextFrame(float delta)
{
	const float kMaxFrameTime = 1.0f / 10.0f;
	if(delta > kMaxFrameTime)
		delta = kMaxFrameTime;
	
	btVector3 ball = m_ball->GetPosition();
	btVector3 ballvel = m_ball->GetLinearVelocity();
	
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
				float desiredHeight = m_desiredHeight * 75.0f;
				
				m_height += (desiredHeight - m_height) * delta * 3.0f;
				m_guide += (m_desiredGuide - m_guide) * delta * 6.0f;
				
				btVector3 forward = m_guide - ball;
				forward.setY(0);
				forward.normalize();
				
				const float kBaseHeight = 12.0f;
				const float kBaseDist = 18.0f;
				
				btVector3 pos = ball - ((m_height + kBaseDist) * forward) + btVector3(0, kBaseHeight + m_height,0);
				btVector3 lookAt = ball + ((m_height + kBaseDist * 2.0f) * forward);
				
				if(m_smooth)
				{
					m_pos += (pos - m_pos) * delta * 3.0f;
					m_lookAt += (lookAt - m_lookAt) * delta * 3.0f;
				}
				else
				{
					m_pos = pos;
					m_lookAt = lookAt;
				}
			}
			break;
		case kPuttCamera:
			{
				float desiredHeight = m_desiredHeight * 10.0f;
				
				m_height += (desiredHeight - m_height) * delta * 3.0f;
				
				btVector3 forward = m_desiredGuide - ball;
				forward.setY(0);
				forward.normalize();
				
				const float kBaseHeight = 12.0f;
				const float kBaseDist = 18.0f;
				
				//btVector3 pos = ball + ((m_height * 1.0f - kBaseDist) * forward) + btVector3(0,kBaseHeight + 4.0f * m_height,0);
				//btVector3 lookAt = ball + ((m_height * -6.0f + kBaseDist * 2.0f) * forward);
				
				btVector3 pos = ball - ((m_height + kBaseDist) * forward) + btVector3(0,kBaseHeight + m_height,0);
				btVector3 lookAt = ball + ((m_height * -3.0f + kBaseDist * 2.0f) * forward);
				
				if(m_smooth)
				{
					m_pos += (pos - m_pos) * delta * 3.0f;
					m_lookAt += (lookAt - m_lookAt) * delta * 3.0f;
				}
				else
				{
					m_pos = pos;
					m_lookAt = lookAt;
				}
			}
			break;
		case kAimCamera:
			{
				float height = 75.0f * m_desiredHeight;
				
				m_lookAt += (m_desiredGuide - m_lookAt) * delta * 3.0f;
				
				btVector3 offset(0,height + 5.0f,18 + height);
				
				btMatrix3x3 rot;
				rot.setEulerYPR(m_yaw, 0.0f, 0.0f);
				offset = rot * offset;
				
				btVector3 newpos = m_desiredGuide + offset;
				
				m_pos += (newpos - m_pos) * delta * 3.0f;
			
			}
			break;
		case kAfterShotCamera:
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
		case kPlacementCamera:
			{
				//ballvel.setY(0.0f);
				//btVector3 lookat = ball + ballvel * 0.5f;
				
				const float kSpeedThreshold = 40.0f;
				const float kMinFOV = 8.0f;
				const float kMaxFOV = 16.0f;
				
				float ballspeed = ballvel.length();
				
				if(ballspeed > kSpeedThreshold)
					ballspeed = kSpeedThreshold;
				
				ballspeed /= kSpeedThreshold;
				ballspeed = 1.0f - ballspeed;
				ballspeed *= (kMaxFOV - kMinFOV);
				
				float halffov = kMaxFOV - ballspeed;
				
				m_halffov += (((halffov / 180.0f) * 3.1415926f) - m_halffov) * delta * 1.0f;
				
				m_lookAt += (ball - m_lookAt) * delta * 3.0f;
				
				m_pos = m_desiredGuide;
			}
			break;
		case kRegardCamera:
			{
				btVector3 forward = m_desiredGuide - ball;
				//float len = forward.length();
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

