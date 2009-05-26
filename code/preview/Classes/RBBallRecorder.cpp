/*
 *  RBBallRecorder.cpp
 *  golf
 *
 *  Created by Robert Rose on 10/10/08.
 *  Copyright 2008 Bork 3D LLC. All rights reserved.
 *
 */

#include "RBBallRecorder.h"
#include "RudeGL.h"
#include "RudeTextureManager.h"

RBBallRecorder::RBBallRecorder()
: m_wrapped(false)
, m_curBallPosition(0)
{
	m_tracerTexture = RudeTextureManager::GetInstance()->LoadTextureFromPVRTFile("tracer");
}


RBBallRecorder::~RBBallRecorder()
{
}

void RBBallRecorder::Reset()
{
	m_wrapped = false;
	m_curBallPosition = 0;
	m_timer = 0.0f;
	m_lastTime = 0.0f;
}

void RBBallRecorder::NextFrame(float delta, bool record)
{
	if(!m_ball)
		return;
	
	m_timer += delta;
	
	if(!record)
		return;
	
	//if(m_timer < 0.1f)
	//	return;
	
	//m_timer = 0.0f;
	
	m_ballPositions[m_curBallPosition].m_position = m_ball->GetPosition();
	m_ballPositions[m_curBallPosition].m_angVel = m_ball->GetAngularVelocity();
	m_ballPositions[m_curBallPosition].m_time = m_timer - m_lastTime;
	
	m_lastTime = m_timer;
	
	m_curBallPosition++;
	
	if(m_curBallPosition >= kNumBallPositions)
	{
		m_wrapped = true;
		m_curBallPosition = 0;
	}
	
	
	
}

void RBBallRecorder::RenderTracers()
{
	float scale = m_ball->GetBallScale();
	
	const float kTracerTimeLen = 0.5f;
	
	float time = 0.0f;
	int tracerLen = 0;
	int k = m_curBallPosition - 1;
	
	while(time < kTracerTimeLen)
	{
		time += m_ballPositions[k].m_time;
		
		k--;
		
		if(k < 0)
		{
			if(m_wrapped)
				k = kNumBallPositions - 1;
			else
				break;
		}
		
		tracerLen++;
	}
	
	RGL.Enable(kDepthTest, true);
	
	RudeTextureManager::GetInstance()->SetTexture(m_tracerTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	RGL.EnableClient(kVertexArray, true);
	RGL.EnableClient(kColorArray, false);
	RGL.EnableClient(kTextureCoordArray, true);
	
	RGL.LoadIdentity();
	
	int i = m_curBallPosition - 1;
	int p = i;
	int c = tracerLen;
	
	bool first = true;
	btVector3 b1;
	btVector3 b2;
	
	float bintensity = 1.0f;
	
	while(c)
	{
		if(p != i)
		{
			btVector3 p1 = m_ballPositions[p].m_position;
			btVector3 p2 = m_ballPositions[i].m_position;
			
			float aintensity = ((float) c) / ((float) tracerLen);
			
			btVector3 dir = p2 - p1;
			dir = dir.normalize();
			btVector3 up(0,1,0);
			
			btVector3 right = dir.cross(up);
			
			right = right.normalize();
			right *= scale;
			
			btVector3 a1 = p1 + right;
			btVector3 a2 = p1 - right;
			
			if(first)
			{
				b1 = p2 + right;
				b2 = p2 - right;
				bintensity = ((float) c-1) / ((float) tracerLen);
				first = false;
			}
			
			GLfloat point[] = {
				a1.x(), a1.y(), a1.z(),
				a2.x(), a2.y(), a2.z(),
				b2.x(), b2.y(), b2.z(),
				b1.x(), b1.y(), b1.z()
			};
			
			
			GLfloat uvs[] = {
				0.0f, aintensity,
				1.0f, aintensity,
				1.0f, bintensity,
				0.0f, bintensity,
			};
			
			glVertexPointer(3, GL_FLOAT, 0, point);
			glTexCoordPointer(2, GL_FLOAT, 0, uvs);
			
			
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
			
			b1 = a1;
			b2 = a2;
			bintensity = aintensity;
			
		}
		
		p = i;
		
		i--;
		c--;
		
		
		if(i < 0)
		{
			if(m_wrapped)
				i = kNumBallPositions - 1;
			else
				c = 0;
		}
	}
}

/*
void RBBallRecorder::RenderTracers()
{
	float scale = m_ball->GetBallScale();
	
	glDisable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	RGL.LoadIdentity();
	
	const int kTracerLen = 10;
	
	int i = m_curBallPosition - 1;
	int c = kTracerLen;
	int p = i;
	
	bool first = true;
	btVector3 b1;
	btVector3 b2;
	
	unsigned int lasta = 0x00;
	
	while(c)
	{
		if(p != i)
		{
			btVector3 p1 = m_ballPositions[p].m_position;
			btVector3 p2 = m_ballPositions[i].m_position;
			
			float intensity = ((float) c) / ((float) kTracerLen);
			
			btVector3 dir = p2 - p1;
			dir = dir.normalize();
			btVector3 up(0,1,0);
			
			btVector3 right = dir.cross(up);
			
			right = right.normalize();
			right *= scale * intensity;
			
			btVector3 a1 = p1 + right;
			btVector3 a2 = p1 - right;
			
			if(first)
			{
				b1 = p2 + right;
				b2 = p2 - right;
				first = false;
			}
			
			GLfloat point[] = {
				a1.x(), a1.y(), a1.z(),
				a2.x(), a2.y(), a2.z(),
				b2.x(), b2.y(), b2.z(),
				b1.x(), b1.y(), b1.z()
			};
			
			unsigned int a = int(intensity * 0xFF) << 24;
			
			unsigned int colors[] = {
				a | 0xFFFFFF,
				a | 0xFFFFFF,
				lasta | 0xFFFFFF,
				lasta | 0xFFFFFF
			};
			
			glVertexPointer(3, GL_FLOAT, 0, point);
			glColorPointer(4, GL_UNSIGNED_BYTE, 0, colors);
			
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
			
			b1 = a1;
			b2 = a2;
			lasta = a;
			
		}
		
		p = i;
		
		i--;
		c--;
		
		
		if(i < 0)
		{
			if(m_wrapped)
				i = kNumBallPositions - 1;
			else
				c = 0;
		}
	}
}
 */

void RBBallRecorder::RenderRecords()
{
	int last = m_curBallPosition;
	
	if(m_wrapped)
		last = kNumBallPositions - 1;
	
	for(int i = 0; i < last; i++)
	{
		RGL.LoadIdentity();
		m_ball->Render(m_ballPositions[i].m_position, m_ballPositions[i].m_angVel * m_timer * 0.2f);
	}

}



