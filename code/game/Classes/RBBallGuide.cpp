/*
 *  RBBallGuide.cpp
 *  golf
 *
 *  Created by Robert Rose on 9/16/08.
 *  Copyright 2008 Bork 3D LLC. All rights reserved.
 *
 */

#include "RBBallGuide.h"
#include "RudeGLD.h"
#include "RudePhysics.h"


#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>

const float kHeightHighlightScale = 15.0f;

void RBBallGuidePoint::SetPoint(const btVector3 &point, const btVector3 &ball, float scale)
{
	m_point = point;
	
	float ydiff = m_point.y() - ball.y();
	ydiff /= scale;
	
	if(ydiff > 1.0f)
		ydiff = 1.0f;
	if(ydiff < -1.0f)
		ydiff = -1.0f;
	
	m_color.Set(1.0f, 1.0f, 1.0f);
	RudeColorFloat blendcolor;
	
	if(ydiff > 0.0f)
	{
		blendcolor.Set(1.0f, 0.0f, 0.0f);
	}
	else
	{
		blendcolor.Set(0.0f, 0.0f, 1.0f);
		ydiff = -ydiff;
	}
		
	m_color.Blend(blendcolor, ydiff);
	
}

RBBallGuide::RBBallGuide()
: m_numGuidePoints(0)
, m_ball(0)
, m_regenCounter(0)
, m_regenAimVec(1,0,0)
, m_regenBall(0,0,0)
, m_regenTimer(0.0f)
{
}

void RBBallGuide::RegenPoints()
{
	m_numGuidePoints = 1;
	
	btDiscreteDynamicsWorld *world = RudePhysics::GetInstance()->GetWorld();
	
	m_regenBall = m_ball->GetPosition();
	
	m_regenAimVec = m_guide - m_regenBall;
	int len = (int) m_regenAimVec.length();
	m_regenAimVec.normalize();
	
	// reset regen counter, will update points during NextFrame()
	m_regenCounter = len;

	btVector3 p = m_guide;
	
	btVector3 p0 = p;
	p0.setY(-1000);
	btVector3 p1 = p;
	p1.setY(1000);
	
	btCollisionWorld::ClosestRayResultCallback cb(p0, p1);
	
	world->rayTest(p0, p1, cb);
	
	if(cb.hasHit())
	{
		RUDE_ASSERT(0 < kMaxGuidePoints, "Out of guide points");
		m_guidePoints[0].SetPoint(cb.m_hitPointWorld, m_regenBall, kHeightHighlightScale);
	}
}

btVector3 RBBallGuide::GetLastGuidePoint()
{
	if(m_numGuidePoints == 0)
		return m_guide;
	else
		return m_guidePoints[0].m_point;
}

void RBBallGuide::NextFrame(float delta)
{
	if(m_regenCounter > 0)
	{
		m_regenTimer += delta;
		
		const float kNumRegensPerSecond = 30.0f;
		const float kRegenPeriod = 1.0f / kNumRegensPerSecond;
		
		while(m_regenTimer > kRegenPeriod && m_regenCounter > 0)
		{
			btDiscreteDynamicsWorld *world = RudePhysics::GetInstance()->GetWorld();
			
			btVector3 p = m_regenBall + (m_regenAimVec * ((float) m_regenCounter));
			
			btVector3 p0 = p;
			p0.setY(-1000);
			btVector3 p1 = p;
			p1.setY(1000);
			
			btCollisionWorld::ClosestRayResultCallback cb(p0, p1);
			
			world->rayTest(p0, p1, cb);
			
			if(cb.hasHit())
			{
				if(m_numGuidePoints < kMaxGuidePoints)
				{
					m_guidePoints[m_numGuidePoints].SetPoint(cb.m_hitPointWorld, m_regenBall, kHeightHighlightScale);
					m_numGuidePoints++;
				}
			}
			
			m_regenCounter -= 9;
			
			if(m_regenCounter < 9)
			{
				if(m_numGuidePoints < kMaxGuidePoints)
				{
					m_guidePoints[m_numGuidePoints].SetPoint(m_regenBall, m_regenBall, kHeightHighlightScale);
					m_numGuidePoints++;
				}
				m_regenCounter = 0;
			}
			
			m_regenTimer -= kRegenPeriod;
		}
		
		
	}
	
}

void RBBallGuide::Render()
{
	RGL.Enable(kDepthTest, false);
	
	//btVector3 ball = m_ball->GetPosition();
	//RGLD.DebugDrawLine(ball, m_guide);
	
	glDisable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	for(int i = 1; i < m_numGuidePoints; i++)
	{
		btVector3 &p0 = m_guidePoints[i-1].m_point;
		btVector3 &p1 = m_guidePoints[i].m_point;
		
		RudeColorFloat c0 = m_guidePoints[i-1].m_color;
		RudeColorFloat c1 = m_guidePoints[i].m_color;
		
		//const float kPointSize = 0.2;
		
		GLfloat point[] = {
			p0.x(), p0.y(), p0.z(),
			p1.x(), p1.y(), p1.z(),
		};
		
		float a = 0.7f;
		
		
		
		GLfloat colors[] = {
			c0.m_r, c0.m_g, c0.m_b, a,
			c1.m_r, c1.m_g, c1.m_b, a,
		};
		
		
		glVertexPointer(3, GL_FLOAT, 0, point);
		glColorPointer(4, GL_FLOAT, 0, colors);
		glDrawArrays(GL_LINES, 0, 4);
	}
}

