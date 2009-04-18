/*
 *  RudeButtonControl.cpp
 *  golf
 *
 *  Created by Robert Rose on 9/16/08.
 *  Copyright 2008 Bork 3D LLC. All rights reserved.
 *
 */

#include "RudeButtonControl.h"
#include "RudeTextureManager.h"

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>



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


void RudeButtonControl::Render()
{
	RudeControl::Render();
	
	RudeTextureManager::GetInstance()->SetTexture(m_stateofftex);
	
	//glDisable(GL_TEXTURE_2D);
	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	int xc = (m_rect.m_right - m_rect.m_left) / 2;
	int yc = (m_rect.m_bottom - m_rect.m_top) / 2;
	float left = xc - m_halfTexsize + m_rect.m_left;
	float top = yc - m_halfTexsize + m_rect.m_top;
	
	GLfloat point[] = {
		left, top + m_texsize,
		left, top,
		left + m_texsize, top,
		left + m_texsize, top + m_texsize
	};
	
	const GLfloat uvs[] = {
		0.0f, 1.0f, 
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
	};
	
	glVertexPointer(2, GL_FLOAT, 0, point);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, uvs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

