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
{
}

void RBBallGuide::RegenPoints(bool fullrez)
{
	m_numGuidePoints = 0;
	
	btDiscreteDynamicsWorld *world = RudePhysics::GetInstance()->GetWorld();
	
	btVector3 ball = m_ball->GetPosition();
	
	btVector3 aimvec = m_guide - ball;
	int len = (int) aimvec.length();
	aimvec.normalize();
	
	if(fullrez)
	{
		for(int i = 3; i < len; i += 9)
		{
			btVector3 p = ball + (aimvec * ((float) i));
			
			btVector3 p0 = p;
			p0.setY(-1000);
			btVector3 p1 = p;
			p1.setY(1000);
			
			btCollisionWorld::ClosestRayResultCallback cb(p0, p1);
			
			world->rayTest(p0, p1, cb);
			
			if(cb.hasHit())
			{
				RUDE_ASSERT(m_numGuidePoints < kMaxGuidePoints, "Out of guide points");
				m_guidePoints[m_numGuidePoints].SetPoint(cb.m_hitPointWorld, ball, kHeightHighlightScale);
				m_numGuidePoints++;
			}
		}
	}
	
	
	btVector3 p = m_guide;
	
	btVector3 p0 = p;
	p0.setY(-1000);
	btVector3 p1 = p;
	p1.setY(1000);
	
	btCollisionWorld::ClosestRayResultCallback cb(p0, p1);
	
	world->rayTest(p0, p1, cb);
	
	if(cb.hasHit())
	{
		RUDE_ASSERT(m_numGuidePoints < kMaxGuidePoints, "Out of guide points");
		m_guidePoints[m_numGuidePoints].SetPoint(cb.m_hitPointWorld, ball, kHeightHighlightScale);
		m_numGuidePoints++;
	}
}

btVector3 RBBallGuide::GetLastGuidePoint()
{
	if(m_numGuidePoints == 0)
		return m_guide;
	else
		return m_guidePoints[m_numGuidePoints-1].m_point;
}

void RBBallGuide::NextFrame(float delta)
{
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

