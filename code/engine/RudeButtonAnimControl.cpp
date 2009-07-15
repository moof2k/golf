/*
 *  RudeButtonAnimControl.cpp
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#include "RudeButtonAnimControl.h"

#include "RudeGL.h"
#include "RudeTextureManager.h"

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>



RudeButtonAnimControl::RudeButtonAnimControl()
: m_state(false)
, m_texsize(64)
, m_halfTexsize(32)
, m_data(0)
, m_numFrames(0)
, m_curFrame(0)
{
}

void RudeButtonAnimControl::SetTextureSize(float size)
{
	m_texsize = size;
	m_halfTexsize = size / 2.0f;
}

bool RudeButtonAnimControl::TouchDown(RudeTouch *t)
{
	if(!RudeControl::TouchDown(t))
		return false;
	
	
	return true;
}

bool RudeButtonAnimControl::TouchMove(RudeTouch *t)
{
	if(!RudeControl::TouchMove(t))
		return false;
	
	return true;
}

bool RudeButtonAnimControl::TouchUp(RudeTouch *t)
{
	if(!RudeControl::TouchUp(t))
		return false;
	
	return true;
}

void RudeButtonAnimControl::SetRect(const RudeRect &r)
{
	m_rect = r;
	
	int xc = (m_rect.m_right - m_rect.m_left) / 2;
	int yc = (m_rect.m_bottom - m_rect.m_top) / 2;
	float left = xc - m_halfTexsize + m_rect.m_left;
	float top = yc - m_halfTexsize + m_rect.m_top;
	
	m_points[0] = left;
	m_points[1] = top + m_texsize;
	
	m_points[2] = left;
	m_points[3] = top;
	
	m_points[4] = left + m_texsize;
	m_points[5] = top;
	
	m_points[6] = left + m_texsize;
	m_points[7] = top + m_texsize;
	
}

void RudeButtonAnimControl::SetAnimData(tRudeButtonAnimKeyframe *data, int numFrames)
{
	RUDE_ASSERT(data, "Invalid animation data");
	RUDE_ASSERT(numFrames > 0, "Invalid animation data");
	
	for(int i = 0; i < numFrames; i++)
	{
		RUDE_ASSERT(data[i].m_time > 0.0f && data[i].m_time < 100.0f, "Unreasoable animation time");
		RUDE_ASSERT(data[i].m_texture >= 0, "Unreasonable animation texture");
	}
	
	m_data = data;
	m_curFrame = 0;
	m_numFrames = numFrames;
}
									

void RudeButtonAnimControl::NextFrame(float delta)
{
	m_time += delta;
	
	while(m_time >= m_data[m_curFrame].m_time)
	{
		m_time -= m_data[m_curFrame].m_time;
		
		m_curFrame++;
		if(m_curFrame >= m_numFrames)
			m_curFrame = 0;
	}
}

void RudeButtonAnimControl::Render()
{
	RudeControl::Render();
	
	tRudeButtonAnimKeyframe *frame = &m_data[m_curFrame];
	
	RudeTextureManager::GetInstance()->SetTexture(frame->m_texture);
	
	//glDisable(GL_TEXTURE_2D);
	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	const GLfloat uvs[] = {
		0.0f, 1.0f, 
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
	};
	
	RGL.EnableClient(kVertexArray, true);
	RGL.EnableClient(kColorArray, false);
	RGL.EnableClient(kTextureCoordArray, true);
	
	glVertexPointer(2, GL_FLOAT, 0, m_points);
	glTexCoordPointer(2, GL_FLOAT, 0, uvs);
	
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}


