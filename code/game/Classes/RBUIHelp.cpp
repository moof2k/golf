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
#include "RudeFont.h"
#include "RudeSound.h"
#include "RudeTextureManager.h"

typedef struct {
	const char *m_text;
	int m_align;
	float m_x, m_y;
} tHelpText;

typedef struct {
	const char *m_textureName;
	int m_numTexts;
	tHelpText m_texts[3];
	
} tHelpPage;

typedef struct {
	int m_numPages;
	tHelpPage m_pages[2];
} tHelpPageInfo;

tHelpPageInfo kHelpPageInfos[kNumHelpModes] = {
{ 1, { "help_a_256", 3, {
	{ "Aim", FONT_ALIGN_CENTER, 160, 192 }, 
	{ "Press", FONT_ALIGN_RIGHT, 310, 375 }, 
	{ "to Swing", FONT_ALIGN_RIGHT, 310, 400 }
	} } },
{ 1, { "help_b_256", 2, {
	{ "Rotate", FONT_ALIGN_CENTER, 160, 350 }, 
	{ "Zoom", FONT_ALIGN_RIGHT, 280, 240 }, 
	} } },
{ 2, {{ "help_c_256", 0, 0 }, { "help_d_256", 0, 0 }} },
};

RBUIHelp::RBUIHelp()
: m_helpTexture(-1)
, m_bgTimer(0.0f)
, m_done(true)
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
	RUDE_ASSERT(kHelpPageInfos[mode].m_numPages > 0, "Invalid number of pages for help");
	
	LoadTexture(kHelpPageInfos[mode].m_pages[0].m_textureName);
	
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
	
	if(m_helpNumber >= kHelpPageInfos[m_mode].m_numPages)
	{
		if(m_helpTexture >= 0)
		{
			RudeTextureManager::GetInstance()->ReleaseTexture(m_helpTexture);
			m_helpTexture = -1;
		}
		
		m_done = true;
	}
	else
		LoadTexture(kHelpPageInfos[m_mode].m_pages[m_helpNumber].m_textureName);
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
		
		const float size = 256.0f;
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
	
	for(int i = 0; i < kHelpPageInfos[m_mode].m_pages[m_helpNumber].m_numTexts; i++)
	{
		tHelpText *helpText = &kHelpPageInfos[m_mode].m_pages[m_helpNumber].m_texts[i];
		
		if(helpText)
		{
			RudeFontManager::GetFont(kBigFontOutline)->Write(helpText->m_x, helpText->m_y, 0.0f, helpText->m_text, 0, helpText->m_align, 0xFF000000, 0xFF000000);
			RudeFontManager::GetFont(kBigFont)->Write(helpText->m_x, helpText->m_y, 0.0f, helpText->m_text, 0, helpText->m_align, 0xFFFFFFFF, 0xFFFFE9CA);
			
		}
	}
}

bool RBUIHelp::TouchDown()
{
	if(m_done)
		return true;
		
	NextHelp();
	
	return false;
}


