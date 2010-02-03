/*
 *  RudeButtonControl.cpp
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#include "RudeButtonControl.h"

#include "RudeGL.h"
#include "RudeTextureManager.h"



RudeButtonControl::RudeButtonControl()
: m_state(false)
, m_stateofftex(-1)
, m_stateontex(-1)
, m_texsize(64)
, m_halfTexsize(32)
{
}

void RudeButtonControl::SetTextures(const char *stateofftex, const char *stateontex)
{
	m_stateofftex = RudeTextureManager::GetInstance()->LoadTextureFromPNGFile(stateofftex);
	m_stateontex = RudeTextureManager::GetInstance()->LoadTextureFromPNGFile(stateontex);
	
	RudeTexture *tex = RudeTextureManager::GetInstance()->GetTexture(m_stateofftex);
	if(tex)
	{
		m_texsize = tex->GetHeight();
		m_halfTexsize = tex->GetHeight() / 2;
	}
}

bool RudeButtonControl::TouchDown(RudeTouch *t)
{
	if(!RudeControl::TouchDown(t))
		return false;

	
	return true;
}

bool RudeButtonControl::TouchMove(RudeTouch *t)
{
	if(!RudeControl::TouchMove(t))
		return false;
	
	return true;
}

bool RudeButtonControl::TouchUp(RudeTouch *t)
{
	if(!RudeControl::TouchUp(t))
		return false;
	
	return true;
}

void RudeButtonControl::SetRect(const RudeRect &r)
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

void RudeButtonControl::Render()
{
	RudeControl::Render();
	
	RudeTextureManager::GetInstance()->SetTexture(m_stateofftex);
	
	const GLfloat uvs[] = {
		0.0f, 1.0f, 
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
	};

	unsigned int colors[] = {
		0xFFFFFFFF,
		0xFFFFFFFF,
		0xFFFFFFFF,
		0xFFFFFFFF
	};
	
	RGL.EnableClient(kVertexArray, true);
	RGL.EnableClient(kColorArray, true);
	RGL.EnableClient(kTextureCoordArray, true);
	
	glVertexPointer(2, GL_FLOAT, 0, m_points);
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, colors);
	glTexCoordPointer(2, GL_FLOAT, 0, uvs);
	
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

