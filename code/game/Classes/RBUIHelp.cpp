/*
 *  RBUIHelp.cpp
 *  golf
 *
 *  Created by Robert Rose on 4/30/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "RBUIHelp.h"

#include "RudeGL.h"
#include "RudeSound.h"
#include "RudeTextureManager.h"

typedef struct {
	int m_numTextures;
	const char *m_textureNames[3];
} tHelpTexInfo;

tHelpTexInfo kHelpTexInfos[kNumHelpModes] = {
	{ 1, { "help_a" } },
	{ 1, { "help_b" } },
	{ 2, { "help_c", "help_d" } },
};

RBUIHelp::RBUIHelp()
: m_helpTexture(-1)
, m_bgTimer(0.0f)
, m_done(true)
, m_enabled(false)
{
	m_done = true;
}

RBUIHelp::~RBUIHelp()
{
	if(m_helpTexture >= 0)
	{
		RudeTextureManager::GetInstance()->ReleaseTexture(m_helpTexture);
		m_helpTexture = -1;
	}
}

void RBUIHelp::SetHelpMode(eHelpMode mode)
{
	m_done = false;
	
	m_mode = mode;
	m_helpNumber = 0;
	RUDE_ASSERT(kHelpTexInfos[mode].m_numTextures > 0, "Invalid number of textures for help");
	
	LoadTexture(kHelpTexInfos[mode].m_textureNames[0]);
	
	m_bgTimer = 0.0f;
}

void RBUIHelp::LoadTexture(const char *texname)
{
	RUDE_REPORT("Help %s\n", texname);
	
	if(m_helpTexture >= 0)
		m_helpTexture = RudeTextureManager::GetInstance()->ReplaceTextureFromPNGFile(m_helpTexture, texname);
	else
		m_helpTexture = RudeTextureManager::GetInstance()->LoadTextureFromPNGFile(texname);
}

void RBUIHelp::NextHelp()
{
	m_helpNumber++;
	
	if(m_helpNumber >= kHelpTexInfos[m_mode].m_numTextures)
	{
		if(m_helpTexture >= 0)
		{
			RudeTextureManager::GetInstance()->ReleaseTexture(m_helpTexture);
			m_helpTexture = -1;
		}
		
		m_done = true;
	}
	else
		LoadTexture(kHelpTexInfos[m_mode].m_textureNames[m_helpNumber]);
}

void RBUIHelp::NextFrame(float delta)
{
	if(m_done)
		return;
	
	m_bgTimer += delta;
}

void RBUIHelp::Render(float aspect)
{
	if(m_done)
		return;
	
	RGL.LoadIdentity();
	
	{
		float alpha = 8.0f * m_bgTimer * 0.5f;
		
		if(alpha > 1.0f)
			alpha = 1.0f;
		
		GLfloat colors[] = {
			1, 1, 1, alpha,
			1, 1, 1, alpha,
			1, 1, 1, alpha,
			1, 1, 1, alpha,
		};
		
		const float size = 512.0f;
		const float halfsize = size / 2.0f;
		
		int xc = 160;
		int yc = 240;
		float left = xc - halfsize + 0;
		float top = yc - halfsize + 0;
		
		float points[8];
		
		points[0] = left;
		points[1] = top + size;
		
		points[2] = left;
		points[3] = top;
		
		points[4] = left + size;
		points[5] = top;
		
		points[6] = left + size;
		points[7] = top + size;
		
		glEnable(GL_TEXTURE_2D);
		
		RudeTextureManager::GetInstance()->SetTexture(m_helpTexture);
		
		RGL.EnableClient(kVertexArray, true);
		RGL.EnableClient(kColorArray, true);
		RGL.EnableClient(kTextureCoordArray, true);
		
		const GLfloat uvs[] = {
			0.0f, 1.0f, 
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
		};
		
		glVertexPointer(2, GL_FLOAT, 0, points);
		glTexCoordPointer(2, GL_FLOAT, 0, uvs);
		glColorPointer(4, GL_FLOAT, 0, colors);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	}
}

bool RBUIHelp::TouchDown()
{
	if(m_done)
		return true;
		
	NextHelp();
	
	return false;
}
