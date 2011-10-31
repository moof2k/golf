/*
 *  RBUIMenu.cpp
 *  golf
 *
 *  Created by Robert Rose on 10/30/08.
 *  Copyright 2008 Bork 3D LLC. All rights reserved.
 *
 */

#include "RBUIMenu.h"
#include "RudeGL.h"
#include "RudeSound.h"

RBUIMenu::RBUIMenu()
: m_ui(0)
{
	if(RUDE_IPAD)
		m_ui.Load("menu_ipad");
	else
		m_ui.Load("menu_iphone");

	m_scoreControl = m_ui.GetChildControl<RBScoreControl>("scoreControl");
	m_scoreControl->SetAnimType(kAnimPopSlide);
	m_scoreControl->SetAnimSpeed(5.0f);
	m_scoreControl->SetActiveHole(0, kCourseAll18);
	
	m_resumeText = m_ui.GetChildControl<RudeTextControl>("resumeText");
	m_resumeText->SetAnimSpeed(5.0f);
	
	m_quitText = m_ui.GetChildControl<RudeTextControl>("quitText");
	m_quitText->SetAnimSpeed(5.0f);	
}

RBUIMenu::~RBUIMenu()
{
}

void RBUIMenu::Reset(int holeNum, eCourseHoles holeset)
{
	m_done = false;
	m_result = kMenuResume;
	
	m_bgTimer = 0.0f;
	
	m_scoreControl->SetTranslation(btVector3(0,-400,0));
	m_scoreControl->SetDesiredTranslation(btVector3(0,0,0));
	
	m_resumeText->SetTranslation(btVector3(0,400,0));
	m_resumeText->SetDesiredTranslation(btVector3(0,0,0));
	
	m_quitText->SetTranslation(btVector3(0,400,0));
	m_quitText->SetDesiredTranslation(btVector3(0,0,0));
	
	m_scoreControl->SetActiveHole(holeNum, holeset);
}

void RBUIMenu::NextFrame(float delta)
{
	m_quitText->NextFrame(delta);
	m_resumeText->NextFrame(delta);
	m_scoreControl->NextFrame(delta);
	
	m_bgTimer += delta;
}

void RBUIMenu::Render(float width, float height)
{
	// splat grey bg
	
	glDisable(GL_TEXTURE_2D);
	
	RGL.EnableClient(kVertexArray, true);
	RGL.EnableClient(kColorArray, true);
	RGL.EnableClient(kTextureCoordArray, false);
	
	GLfloat point[] = {
		0, 0,
		width, 0,
		width, height,
		0, height
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

	m_scoreControl->Render();
	
	m_quitText->Render();
	m_resumeText->Render();
	
}

void RBUIMenu::TouchDown(RudeTouch *rbt)
{
	m_quitText->TouchDown(rbt);
	m_resumeText->TouchDown(rbt);
}

void RBUIMenu::TouchMove(RudeTouch *rbt)
{
}

void RBUIMenu::TouchUp(RudeTouch *rbt)
{
	eSoundEffect sfx = kSoundNone;

	if(m_quitText->TouchUp(rbt))
	{
		m_result = kMenuQuit;
		m_done = true;
		
		sfx = kSoundUIExit;
	}
	else if(m_resumeText->TouchUp(rbt))
	{
		m_result = kMenuResume;
		m_done = true;
		
		sfx = kSoundUIBack;
	}
	
	RudeSound::GetInstance()->PlayWave(sfx);
}

void RBUIMenu::Resize()
{
	if(RUDE_IPAD)
	{
		int width = (int) RGL.GetDeviceWidth();
		int center = width / 2;
		int offset = 768 / 2;
		m_ui.SetFileRect(RudeRect(0, center - offset, -1, center + offset));
	}

	m_ui.UpdateDrawRect();
}