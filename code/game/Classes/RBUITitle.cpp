/*
 *  RBUITitle.cpp
 *  golf
 *
 *  Created by Robert Rose on 10/29/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "RBUITitle.h"
#include "RudeGL.h"

class RBCourseEntry {
public:
	const char *m_name;
	eCourseHoles m_holes;
	eCourseTee m_tee;
	
};

static RBCourseEntry sCourseData[kNumCourses] = {
	{ "Creekside Country Club", kCourseFront9, kCourseShortTee },
	{ "Creekside Country Club", kCourseBack9, kCourseShortTee },
	{ "Creekside Country Club", kCourseFront9, kCourseLongTee },
	{ "Creekside Country Club", kCourseBack9, kCourseLongTee },
	{ "Creekside Country Club", kCourseAll18, kCourseShortTee },
	{ "Creekside Country Club", kCourseAll18, kCourseLongTee }
};


RBUITitle::RBUITitle()
{
	m_terrain.LoadMesh("parfive");
	m_skybox.Load("skybox");
	
	m_logo.SetAnimType(kAnimPopSlide);
	m_logo.SetRect(RudeRect(80, 0, 300, 320));
	m_logo.SetTextures("logo", "logo");
	
	const int kCourseButtonTop = 30;
	const int kCourseButtonHeight = 64;
	const float kCourseAnimSpeedBase = 3.0f;
	const float kCourseAnimSpeedMod = -0.2f;
	for(int i = 0; i < kNumCourses; i++)
	{
		int offset = kCourseButtonTop + kCourseButtonHeight * i;
		
		m_courseButtons[i].SetAnimType(kAnimPopSlide);
		m_courseButtons[i].SetAnimSpeed(float(i) * kCourseAnimSpeedMod + kCourseAnimSpeedBase);
		m_courseButtons[i].SetRect(RudeRect(offset, 0, offset + kCourseButtonHeight, 320));
		m_courseButtons[i].SetTextures("coursebg", "coursebg");
		
		m_courseButtons[i].m_name = sCourseData[i].m_name;
		m_courseButtons[i].m_holes = sCourseData[i].m_holes;
		m_courseButtons[i].m_tee = sCourseData[i].m_tee;
	}
	
	m_startText.SetAnimType(kAnimPopSlide);
	m_startText.SetText("Start");
	m_startText.SetAlignment(kAlignCenter);
	m_startText.SetRect(RudeRect(320, 0, 340, 320));
	m_startText.SetStyle(kOutlineStyle);
	m_startText.SetFont(kBigFont);
	m_startText.SetColors(0, 0xFFFFFFFF, 0xFFCCCCCC);
	m_startText.SetColors(1, 0xFF000000, 0xFF000000);
	
	m_continueText.SetAnimType(kAnimPopSlide);
	m_continueText.SetText("Continue");
	m_continueText.SetAlignment(kAlignCenter);
	m_continueText.SetRect(RudeRect(350, 0, 370, 320));
	m_continueText.SetStyle(kOutlineStyle);
	m_continueText.SetFont(kBigFont);
	m_continueText.SetColors(0, 0xFFFFFFFF, 0xFFCCCCCC);
	m_continueText.SetColors(1, 0xFF000000, 0xFF000000);
	
	m_backText.SetAnimType(kAnimPopSlide);
	m_backText.SetText("Back");
	m_backText.SetAlignment(kAlignCenter);
	m_backText.SetRect(RudeRect(440, 0, 460, 320));
	m_backText.SetStyle(kOutlineStyle);
	m_backText.SetFont(kBigFont);
	m_backText.SetColors(0, 0xFFFFFFFF, 0xFFCCCCCC);
	m_backText.SetColors(1, 0xFF000000, 0xFF000000);
	
	m_copyrightText.SetAnimType(kAnimPopSlide);
	m_copyrightText.SetText("Copyright © 2008 Bork 3D LLC");
	m_copyrightText.SetAlignment(kAlignCenter);
	m_copyrightText.SetRect(RudeRect(463, 0, 480, 320));
	m_copyrightText.SetStyle(kNoStyle);
	m_copyrightText.SetColors(0, 0xFFFFFFFF, 0xFFCCCCCC);
	m_copyrightText.SetColors(1, 0xFF000000, 0xFF000000);
	
	m_cameraTimer = 0.0f;
	
	m_state = kTitleNone;
	SetState(kTitleSplash);
}

RBUITitle::~RBUITitle()
{
}

void RBUITitle::SetState(eTitleState state)
{
	switch(m_state)
	{
		case kTitleNone:
			
			m_backText.SetTranslation(btVector3(400,0,0));
			m_backText.SetDesiredTranslation(btVector3(400,0,0));
			for(int i = 0; i < kNumCourses; i++)
			{
				m_courseButtons[i].SetTranslation(btVector3(400,0,0));
				m_courseButtons[i].SetDesiredTranslation(btVector3(400,0,0));
			}
			
			break;
		case kTitleSplash:
			m_logo.SetDesiredTranslation(btVector3(-400,0,0));
			m_startText.SetDesiredTranslation(btVector3(-400,0,0));
			m_continueText.SetDesiredTranslation(btVector3(-400,0,0));
			m_copyrightText.SetDesiredTranslation(btVector3(0,100,0));
			
			break;
		case kTitleCourseSelect:
			m_backText.SetDesiredTranslation(btVector3(400,0,0));
			for(int i = 0; i < kNumCourses; i++)
			{
				m_courseButtons[i].SetDesiredTranslation(btVector3(400,0,0));
			}
			break;
	}
	
	m_state = state;
	
	switch(m_state)
	{
		case kTitleSplash:
			
			m_logo.SetDesiredTranslation(btVector3(0,0,0));
			m_startText.SetDesiredTranslation(btVector3(0,0,0));
			m_continueText.SetDesiredTranslation(btVector3(0,0,0));
			m_copyrightText.SetDesiredTranslation(btVector3(0,0,0));
			break;
		case kTitleCourseSelect:
			m_backText.SetDesiredTranslation(btVector3(0,0,0));
			for(int i = 0; i < kNumCourses; i++)
			{
				m_courseButtons[i].SetDesiredTranslation(btVector3(0,0,0));
			}
			break;
		
	}
}

void RBUITitle::NextFrame(float delta)
{
	m_cameraTimer += delta;
	
	btVector3 camoff(50,0,0);
	btMatrix3x3 mat;
	mat.setEulerYPR(m_cameraTimer * 0.01f, 0.0f, 0.0f);
	camoff = mat * camoff;
	
	m_camera.SetPos(camoff);
	
	
	
	
	m_logo.NextFrame(delta);
	m_startText.NextFrame(delta);
	m_continueText.NextFrame(delta);
	m_copyrightText.NextFrame(delta);
	m_backText.NextFrame(delta);
	
	for(int i = 0; i < kNumCourses; i++)
	{
		m_courseButtons[i].NextFrame(delta);
	}
}

void RBUITitle::Render(float aspect)
{
	RGL.SetViewport(0, 0, 480, 320);
	
	m_camera.SetView(aspect);
	RGL.LoadIdentity();
	
	m_skybox.Render();
	m_terrain.Render();
	
	RGL.Ortho(0.0f, 0.0f, 0.0f, 320.0f, 480.0f, 100.0f);
	RGL.LoadIdentity();
	RGL.Enable(kBackfaceCull, false);
	RGL.Enable(kDepthTest, false);
	
	
	m_logo.Render();
	m_startText.Render();
	m_continueText.Render();
	m_backText.Render();
	
	for(int i = 0; i < kNumCourses; i++)
	{
		m_courseButtons[i].Render();
	}
	
	m_copyrightText.Render();
}

void RBUITitle::TouchDown(RudeTouch *rbt)
{
	switch(m_state)
	{
		case kTitleSplash:
			m_startText.TouchDown(rbt);
			break;
		case kTitleCourseSelect:
			m_backText.TouchDown(rbt);
			break;
	}
}

void RBUITitle::TouchMove(RudeTouch *rbt)
{
}

void RBUITitle::TouchUp(RudeTouch *rbt)
{
	switch(m_state)
	{
		case kTitleSplash:
			if(m_startText.TouchUp(rbt))
				SetState(kTitleCourseSelect);
			break;
		case kTitleCourseSelect:
			if(m_backText.TouchUp(rbt))
				SetState(kTitleSplash);
			break;
	}
}

