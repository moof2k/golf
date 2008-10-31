/*
 *  RBUIMenu.cpp
 *  golf
 *
 *  Created by Robert Rose on 10/30/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "RBUIMenu.h"
#include "RudeGL.h"

RBUIMenu::RBUIMenu()
{
	m_resumeText.SetAnimType(kAnimPopSlide);
	m_resumeText.SetAnimSpeed(5.0f);
	m_resumeText.SetText("Resume");
	m_resumeText.SetAlignment(kAlignCenter);
	m_resumeText.SetRect(RudeRect(200, 0, 220, 320));
	m_resumeText.SetStyle(kOutlineStyle);
	m_resumeText.SetFont(kBigFont);
	m_resumeText.SetColors(0, 0xFFFFFFFF, 0xFFCCCCCC);
	m_resumeText.SetColors(1, 0xFF000000, 0xFF000000);
	
	m_quitText.SetAnimType(kAnimPopSlide);
	m_quitText.SetAnimSpeed(5.0f);
	m_quitText.SetText("Quit");
	m_quitText.SetAlignment(kAlignCenter);
	m_quitText.SetRect(RudeRect(250, 0, 270, 320));
	m_quitText.SetStyle(kOutlineStyle);
	m_quitText.SetFont(kBigFont);
	m_quitText.SetColors(0, 0xFFFFFFFF, 0xFFCCCCCC);
	m_quitText.SetColors(1, 0xFF000000, 0xFF000000);	
	
	
	
	
}

RBUIMenu::~RBUIMenu()
{
}

void RBUIMenu::Reset()
{
	m_done = false;
	m_result = kMenuResume;
	
	m_bgTimer = 0.0f;
	
	m_resumeText.SetTranslation(btVector3(0,400,0));
	m_resumeText.SetDesiredTranslation(btVector3(0,0,0));
	
	m_quitText.SetTranslation(btVector3(0,400,0));
	m_quitText.SetDesiredTranslation(btVector3(0,0,0));
	
}

void RBUIMenu::NextFrame(float delta)
{
	m_quitText.NextFrame(delta);
	m_resumeText.NextFrame(delta);
	
	m_bgTimer += delta;
}

void RBUIMenu::Render(float aspect)
{
	// splat grey bg
	
	glDisable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	GLfloat point[] = {
		0, 0,
		320, 0,
		320, 480,
		0, 480
	};
	
	float alpha = 4.0f * m_bgTimer * 0.5f;
	
	if(alpha > 0.5f)
		alpha = 0.5f;
	
	GLfloat colors[] = {
		0.0f, 0.0f, 0.0f, alpha,
		0.0f, 0.0f, 0.0f, alpha,
		0.0f, 0.0f, 0.0f, alpha,
		0.0f, 0.0f, 0.0f, alpha,
	};
	
	
	glVertexPointer(2, GL_FLOAT, 0, point);
	glColorPointer(4, GL_FLOAT, 0, colors);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	
	m_quitText.Render();
	m_resumeText.Render();
	
}

void RBUIMenu::TouchDown(RudeTouch *rbt)
{
	m_quitText.TouchDown(rbt);
	m_resumeText.TouchDown(rbt);
}

void RBUIMenu::TouchMove(RudeTouch *rbt)
{
}

void RBUIMenu::TouchUp(RudeTouch *rbt)
{
	if(m_quitText.TouchUp(rbt))
	{
		m_result = kMenuQuit;
		m_done = true;
	}
	else if(m_resumeText.TouchUp(rbt))
	{
		m_result = kMenuResume;
		m_done = true;
	}
}

