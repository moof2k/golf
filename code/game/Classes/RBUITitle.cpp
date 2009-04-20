/*
 *  RBUITitle.cpp
 *  golf
 *
 *  Created by Robert Rose on 10/29/08.
 *  Copyright 2008 Bork 3D LLC. All rights reserved.
 *
 */

#include "RBUITitle.h"
#include "RudeGL.h"
#include "RBCourseData.h"
#include "RBTourTracker.h"

#include "RudeSound.h"

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>



const btVector3 kFlagPosition(-5.741,-18.27,-1572.807);

RBUITitle::RBUITitle()
{
	m_flagOffset = 0.0f;
	
	m_startedMusic = false;
	
	m_terrain.LoadMesh("par5_A");
	m_skybox.Load("skybox");
	
	m_flag.LoadMesh("sexy");
	m_flag.SetPosition(kFlagPosition);
	
	m_logo.SetAnimType(kAnimPopSlide);
	m_logo.SetTextures("logo", "logo");
	m_logo.SetRect(RudeRect(60, 0, 300, 320));
	
	m_scoreControl.SetAnimType(kAnimPopSlide);
	m_scoreControl.SetRect(RudeRect(0,0,480,320));
	m_scoreControl.SetActiveHole(0, kCourseAll18);
	
	m_courseMedallion.SetAnimType(kAnimPopSlide);
	m_courseMedallion.SetTextures("ui_ccc_medallion_128", "ui_ccc_medallion_128");
	m_courseMedallion.SetRect(RudeRect(260, 160, 356, 320));
	
	m_courseSplash.SetAnimType(kAnimPopSlide);
	m_courseSplash.SetTextures("ui_wreath_gold", "ui_wreath_gold");
	m_courseSplash.SetRect(RudeRect(60, 0, 356, 240));
	
	const int kCourseButtonTop = 66;
	const int kCourseButtonHeight = 58;
	const float kCourseAnimSpeedBase = 3.0f;
	const float kCourseAnimSpeedMod = -0.2f;
	for(int i = 0; i < kNumCourses; i++)
	{
		int offset = kCourseButtonTop + kCourseButtonHeight * i;
		
		m_courseButtons[i].SetAnimType(kAnimPopSlide);
		m_courseButtons[i].SetAnimSpeed(float(i) * kCourseAnimSpeedMod + kCourseAnimSpeedBase);
		m_courseButtons[i].SetTextures("coursebg", "coursebg");
		m_courseButtons[i].SetRect(RudeRect(offset, 0, offset + kCourseButtonHeight, 320));
		
		m_courseButtons[i].m_name = sCourseData[i].m_name;
		m_courseButtons[i].m_subname = sCourseData[i].m_subname;
		m_courseButtons[i].m_desc = sCourseData[i].m_desc;
		m_courseButtons[i].m_image = sCourseData[i].m_image;
		m_courseButtons[i].m_imageOffset = sCourseData[i].m_imageOffset;
		m_courseButtons[i].m_holes = sCourseData[i].m_holes;
		m_courseButtons[i].m_tee = sCourseData[i].m_tee;
		m_courseButtons[i].m_completed = false;
	}
	
	m_startText.SetAnimType(kAnimPopSlide);
	m_startText.SetText("Play");
	m_startText.SetAlignment(kAlignCenter);
	m_startText.SetRect(RudeRect(320, 0, 350, 320));
	m_startText.SetStyle(kOutlineStyle);
	m_startText.SetFont(kBigFont);
	m_startText.SetColors(0, 0xFFFFFFFF, 0xFFCCCCCC);
	m_startText.SetColors(1, 0xFF000000, 0xFF000000);
	
	m_practiceText.SetAnimType(kAnimPopSlide);
	m_practiceText.SetText("Practice");
	m_practiceText.SetAlignment(kAlignCenter);
	m_practiceText.SetRect(RudeRect(360, 0, 390, 320));
	m_practiceText.SetStyle(kOutlineStyle);
	m_practiceText.SetFont(kBigFont);
	m_practiceText.SetColors(0, 0xFFFFFFFF, 0xFFCCCCCC);
	m_practiceText.SetColors(1, 0xFF000000, 0xFF000000);
	
	m_goText.SetAnimType(kAnimPopSlide);
	m_goText.SetText("Let's Go!");
	m_goText.SetAlignment(kAlignCenter);
	m_goText.SetRect(RudeRect(400, 0, 420, 320));
	m_goText.SetStyle(kOutlineStyle);
	m_goText.SetFont(kBigFont);
	m_goText.SetColors(0, 0xFFFFFFFF, 0xFFCCCCCC);
	m_goText.SetColors(1, 0xFF000000, 0xFF000000);	
	
	m_backText.SetAnimType(kAnimPopSlide);
	m_backText.SetText("Back");
	m_backText.SetAlignment(kAlignCenter);
	m_backText.SetRect(RudeRect(440, 0, 460, 320));
	m_backText.SetStyle(kOutlineStyle);
	m_backText.SetFont(kBigFont);
	m_backText.SetColors(0, 0xFFFFFFFF, 0xFFCCCCCC);
	m_backText.SetColors(1, 0xFF000000, 0xFF000000);
	
	m_copyrightText.SetAnimType(kAnimPopSlide);
	m_copyrightText.SetText("Copyright © 2009 Bork 3D LLC");
	m_copyrightText.SetAlignment(kAlignCenter);
	m_copyrightText.SetRect(RudeRect(463, 0, 480, 320));
	m_copyrightText.SetStyle(kNoStyle);
	m_copyrightText.SetColors(0, 0xFFFFFFFF, 0xFFCCCCCC);
	m_copyrightText.SetColors(1, 0xFF000000, 0xFF000000);
	
	m_tournamentText.SetAnimType(kAnimPopSlide);
	m_tournamentText.SetText("Select Tournament");
	m_tournamentText.SetAlignment(kAlignCenter);
	m_tournamentText.SetRect(RudeRect(36, 0, 36 + 20, 320));
	m_tournamentText.SetStyle(kOutlineStyle);
	m_tournamentText.SetFont(kBigFont);
	m_tournamentText.SetColors(0, 0xFFFFFFFF, 0xFFCCCCCC);
	m_tournamentText.SetColors(1, 0xFF000000, 0xFF000000);
	
	
	m_courseNameText.SetAnimType(kAnimPopSlide);
	m_courseNameText.SetText("Golf Course");
	m_courseNameText.SetAlignment(kAlignCenter);
	m_courseNameText.SetRect(RudeRect(36, 0, 36 + 20, 320));
	m_courseNameText.SetStyle(kOutlineStyle);
	m_courseNameText.SetFont(kBigFont);
	m_courseNameText.SetColors(0, 0xFFFFFFFF, 0xFFCCCCCC);
	m_courseNameText.SetColors(1, 0xFF000000, 0xFF000000);
	
	m_courseSubnameText.SetAnimType(kAnimPopSlide);
	m_courseSubnameText.SetText("Amateur Invitational");
	m_courseSubnameText.SetAlignment(kAlignCenter);
	m_courseSubnameText.SetRect(RudeRect(60, 0, 60 + 20, 320));
	m_courseSubnameText.SetStyle(kOutlineStyle);
	m_courseSubnameText.SetFont(kBigFont);
	m_courseSubnameText.SetColors(0, 0xFFFFFFFF, 0xFFCCCCCC);
	m_courseSubnameText.SetColors(1, 0xFF000000, 0xFF000000);
	
	m_courseDescText.SetAnimType(kAnimPopSlide);
	m_courseDescText.SetText("Front 9 / Short Tee");
	m_courseDescText.SetAlignment(kAlignCenter);
	m_courseDescText.SetRect(RudeRect(80, 0, 80 + 20, 320));
	m_courseDescText.SetStyle(kOutlineStyle);
	m_courseDescText.SetFont(kDefaultFont);
	m_courseDescText.SetColors(0, 0xFFFFFFFF, 0xFFCCCCCC);
	m_courseDescText.SetColors(1, 0xFF000000, 0xFF000000);
	
	
	m_scoreText.SetAnimType(kAnimPopSlide);
	m_scoreText.SetText("+1");
	m_scoreText.SetAlignment(kAlignCenter);
	m_scoreText.SetRect(RudeRect(400, 0, 420, 320));
	m_scoreText.SetStyle(kOutlineStyle);
	m_scoreText.SetFont(kBigFont);
	m_scoreText.SetColors(0, 0xFFFFFFFF, 0xFFCCCCCC);
	m_scoreText.SetColors(1, 0xFF000000, 0xFF000000);	
	
	m_scoreDesc.SetAnimType(kAnimPopSlide);
	m_scoreDesc.SetText("Congratulations!");
	m_scoreDesc.SetAlignment(kAlignCenter);
	m_scoreDesc.SetRect(RudeRect(440, 0, 460, 320));
	m_scoreDesc.SetStyle(kOutlineStyle);
	m_scoreDesc.SetFont(kDefaultFont);
	m_scoreDesc.SetColors(0, 0xFFFFFFFF, 0xFFCCCCCC);
	m_scoreDesc.SetColors(1, 0xFF000000, 0xFF000000);
	
	m_cameraTimer = 0.0f;
	
	m_course = 0;
	
	Reset();
}

RBUITitle::~RBUITitle()
{
}

void RBUITitle::Reset()
{
	m_startedMusic = false;
	m_done = false;
	m_state = kTitleNone;
	SetState(kTitleSplash);
}

void RBUITitle::SetState(eTitleState state)
{
	RUDE_REPORT("RBUITitle::SetState %d => %d\n", m_state, state);
	
	switch(m_state)
	{
		case kTitleNone:
			
			RefreshScores();
			
			m_logo.SetTranslation(btVector3(0,0,0));
			m_startText.SetTranslation(btVector3(0,0,0));
			m_practiceText.SetTranslation(btVector3(0,0,0));
			
			m_scoreControl.SetTranslation(btVector3(-400,0,0));
			m_scoreControl.SetDesiredTranslation(btVector3(-400,0,0));
			
			m_courseMedallion.SetTranslation(btVector3(400,0,0));
			m_courseMedallion.SetDesiredTranslation(btVector3(400,0,0));
			
			m_courseSplash.SetTranslation(btVector3(400,0,0));
			m_courseSplash.SetDesiredTranslation(btVector3(400,0,0));
			
			m_backText.SetTranslation(btVector3(400,0,0));
			m_backText.SetDesiredTranslation(btVector3(400,0,0));
			
			m_tournamentText.SetTranslation(btVector3(400,0,0));
			m_tournamentText.SetDesiredTranslation(btVector3(400,0,0));
			
			for(int i = 0; i < kNumCourses; i++)
			{
				m_courseButtons[i].SetTranslation(btVector3(400,0,0));
				m_courseButtons[i].SetDesiredTranslation(btVector3(400,0,0));
			}
			
			m_courseNameText.SetTranslation(btVector3(400,0,0));
			m_courseSubnameText.SetTranslation(btVector3(400,0,0));
			m_courseDescText.SetTranslation(btVector3(400,0,0));
			m_courseNameText.SetDesiredTranslation(btVector3(400,0,0));
			m_courseSubnameText.SetDesiredTranslation(btVector3(400,0,0));
			m_courseDescText.SetDesiredTranslation(btVector3(400,0,0));
			
			m_goText.SetTranslation(btVector3(400,0,0));
			m_goText.SetDesiredTranslation(btVector3(400,0,0));
			
			m_scoreText.SetDesiredTranslation(btVector3(-400,0,0));
			m_scoreDesc.SetDesiredTranslation(btVector3(-400,0,0));
			m_scoreText.SetTranslation(btVector3(-400,0,0));
			m_scoreDesc.SetTranslation(btVector3(-400,0,0));
						
			break;
		case kTitleScoreSummary:
			RefreshScores();
			
			m_scoreControl.SetDesiredTranslation(btVector3(-400,0,0));
			
			m_courseNameText.SetDesiredTranslation(btVector3(-400,0,0));
			m_courseSubnameText.SetDesiredTranslation(btVector3(-400,0,0));
			m_courseDescText.SetDesiredTranslation(btVector3(-400,0,0));
			
			m_scoreText.SetDesiredTranslation(btVector3(-400,0,0));
			m_scoreDesc.SetDesiredTranslation(btVector3(-400,0,0));
			
			break;
		case kTitleSplash:
			RefreshScores();
			
			m_logo.SetDesiredTranslation(btVector3(-400,0,0));
			m_startText.SetDesiredTranslation(btVector3(-400,0,0));
			m_practiceText.SetDesiredTranslation(btVector3(-400,0,0));
			m_copyrightText.SetDesiredTranslation(btVector3(0,100,0));
			
			break;
		case kTitleCourseSelect:
		{
			
			float dir = 1.0f;
			if(state == kTitleGameOptions)
			{
				dir = -1.0f;
			}
			else
				m_backText.SetDesiredTranslation(btVector3(dir * 400,0,0));
			
			for(int i = 0; i < kNumCourses; i++)
			{
				m_courseButtons[i].SetDesiredTranslation(btVector3(dir * 400,0,0));
			}
			
			m_tournamentText.SetDesiredTranslation(btVector3(dir * 400,0,0));
		}
			break;
		case kTitleGameOptions:
			if(state != kTitleReadyToPlay)
			{
				m_goText.SetDesiredTranslation(btVector3(400,0,0));
				
				m_courseNameText.SetDesiredTranslation(btVector3(400,0,0));
				m_courseSubnameText.SetDesiredTranslation(btVector3(400,0,0));
				m_courseDescText.SetDesiredTranslation(btVector3(400,0,0));
				
				m_courseMedallion.SetDesiredTranslation(btVector3(400,0,0));
				m_courseSplash.SetDesiredTranslation(btVector3(400,m_courseButtons[m_course].GetImageOffset(),0));
			}
			
			break;
			
		case kTitlePracticeOptions:
			m_courseNameText.SetDesiredTranslation(btVector3(400,0,0));
			
			m_goText.SetDesiredTranslation(btVector3(400,0,0));
			m_backText.SetDesiredTranslation(btVector3(400,0,0));
			break;
	}
	
	m_state = state;
	
	switch(m_state)
	{
		case kTitleScoreSummary:
			
			
			
			m_logo.SetTranslation(btVector3(400,0,0));
			m_startText.SetTranslation(btVector3(400,0,0));
			m_practiceText.SetTranslation(btVector3(400,0,0));
			m_logo.SetDesiredTranslation(btVector3(400,0,0));
			m_startText.SetDesiredTranslation(btVector3(400,0,0));
			m_practiceText.SetDesiredTranslation(btVector3(400,0,0));
			
			m_scoreControl.SetActiveHole(-1, m_courseButtons[m_course].m_holes);
			m_scoreControl.SetDesiredTranslation(btVector3(0,0,0));
			
			m_courseNameText.SetTranslation(btVector3(-400,0,0));
			m_courseSubnameText.SetTranslation(btVector3(-400,0,0));
			m_courseDescText.SetTranslation(btVector3(-400,0,0));
			m_courseNameText.SetDesiredTranslation(btVector3(0,0,0));
			m_courseSubnameText.SetDesiredTranslation(btVector3(0,0,0));
			m_courseDescText.SetDesiredTranslation(btVector3(0,0,0));
			
			RUDE_ASSERT(m_course >= 0, "Invalid course");
			m_courseNameText.SetText(m_courseButtons[m_course].GetNameStr());
			m_courseSubnameText.SetText(m_courseButtons[m_course].GetSubnameStr());
			m_courseDescText.SetText(m_courseButtons[m_course].GetDescStr());
			
			m_scoreText.SetDesiredTranslation(btVector3(0,0,0));
			m_scoreDesc.SetDesiredTranslation(btVector3(0,0,0));
			
			break;
			
		case kTitleSplash:
			
			m_logo.SetDesiredTranslation(btVector3(0,0,0));
			m_startText.SetDesiredTranslation(btVector3(0,0,0));
			m_practiceText.SetDesiredTranslation(btVector3(0,0,0));
			m_copyrightText.SetDesiredTranslation(btVector3(0,0,0));
			break;
			
		case kTitleCourseSelect:
			m_courseNameText.SetTranslation(btVector3(400,0,0));
			m_courseSubnameText.SetTranslation(btVector3(400,0,0));
			m_courseDescText.SetTranslation(btVector3(400,0,0));
			m_courseNameText.SetDesiredTranslation(btVector3(400,0,0));
			m_courseSubnameText.SetDesiredTranslation(btVector3(400,0,0));
			m_courseDescText.SetDesiredTranslation(btVector3(400,0,0));
			
			m_tournamentText.SetDesiredTranslation(btVector3(0,0,0));
			
			m_backText.SetDesiredTranslation(btVector3(0,0,0));
			for(int i = 0; i < kNumCourses; i++)
			{
				m_courseButtons[i].SetDesiredTranslation(btVector3(0,0,0));
			}
			break;
			
		case kTitleGameOptions:
			m_goText.SetDesiredTranslation(btVector3(0,0,0));
			
			m_courseNameText.SetDesiredTranslation(btVector3(0,0,0));
			m_courseSubnameText.SetDesiredTranslation(btVector3(0,0,0));
			m_courseDescText.SetDesiredTranslation(btVector3(0,0,0));
			
			RUDE_ASSERT(m_course >= 0, "Invalid course");
			m_courseNameText.SetText(m_courseButtons[m_course].GetNameStr());
			m_courseSubnameText.SetText(m_courseButtons[m_course].GetSubnameStr());
			m_courseDescText.SetText(m_courseButtons[m_course].GetDescStr());
			
			m_courseSplash.SetTextures(m_courseButtons[m_course].GetImageStr(), m_courseButtons[m_course].GetImageStr());
			
			m_courseMedallion.SetDesiredTranslation(btVector3(0,0,0));
			m_courseSplash.SetDesiredTranslation(btVector3(0,m_courseButtons[m_course].GetImageOffset(),0));
			
			break;
			
		case kTitlePracticeOptions:
			m_goText.SetDesiredTranslation(btVector3(0,0,0));
			m_backText.SetDesiredTranslation(btVector3(0,0,0));
			
			m_courseNameText.SetText(sCourseData[kNumCourses - 1].m_name);
			
			m_courseNameText.SetDesiredTranslation(btVector3(0,0,0));
			
			break;
			
		case kTitleReadyToPlay:
			RudeSound::GetInstance()->BgmVolFade(-0.5f);
			
			m_readyTimer = 0.0f;
			m_goText.SetDesiredTranslation(btVector3(0,100,0));
			m_backText.SetDesiredTranslation(btVector3(0,100,0));
			break;
			
		case kTitleReadyToPractice:
			RudeSound::GetInstance()->BgmVolFade(-0.5f);
			
			m_readyTimer = 0.0f;
			m_goText.SetDesiredTranslation(btVector3(0,100,0));
			m_backText.SetDesiredTranslation(btVector3(0,100,0));
			break;
		
	}
}

void RBUITitle::RefreshScores()
{
	for(int i = 0; i < kNumCourses; i++)
	{
		bool unlocked = RBTourTracker::Unlocked(i);
		
		if(unlocked)
		{
			int score = RBTourTracker::GetScore(i);
		
			char str[32];
			
			if(score == 0)
				sprintf(str, "±0");
			else if(score > 99)
				sprintf(str, "", score);
			else if(score > 9)
				sprintf(str, "+%d", score);
			else if(score > 0)
				sprintf(str, " +%d", score);
			else
				sprintf(str, " %d", score);
			
			strcpy(m_courseButtons[i].m_scoreText, str);
			
			m_courseButtons[i].m_name = sCourseData[i].m_name;
			m_courseButtons[i].m_subname = sCourseData[i].m_subname;
			m_courseButtons[i].SetTextures("coursebg", "coursebg");
			
			if(RBTourTracker::Completed(i))
				m_courseButtons[i].m_completed = true;
			else
				m_courseButtons[i].m_completed = false;
		}
		else
		{
			m_courseButtons[i].SetTextures("coursebg_locked", "coursebg_locked");
			
			m_courseButtons[i].m_name = "";
			m_courseButtons[i].m_subname = "";
			m_courseButtons[i].m_completed = false;
		}
	}
}

void RBUITitle::SetCourseScore(int score)
{
	char str[32];
	
	if(score == 0)
		sprintf(str, "±0");
	else if(score > 0)
		sprintf(str, "+%d", score);
	else
		sprintf(str, "%d", score);
	
	m_scoreText.SetText(str);
}

void RBUITitle::NextFrame(float delta)
{
	if(!m_startedMusic)
	{
		RudeSound::GetInstance()->BgmVol(0.5f);
		RudeSound::GetInstance()->PlaySong(kBGMTitle);
		m_startedMusic = true;
	}
	
	m_cameraTimer += delta;
	
	if(m_state == kTitleReadyToPlay || m_state == kTitleReadyToPractice)
	{
		m_readyTimer += delta;
		
		if(m_readyTimer > 1.5f)
		{
			m_startedMusic = false;
			m_done = true;
		}
	}
	
	float desiredFlagOffset = -12.0f;
	
	if(m_state == kTitleSplash)
		desiredFlagOffset = 0.0f;
	
	m_flagOffset += (desiredFlagOffset - m_flagOffset) * delta * 5.0f;
	
	m_flag.SetPosition(kFlagPosition + btVector3(0, m_flagOffset, 0));
	
	
	btVector3 lookat(kFlagPosition);
	lookat += btVector3(0.0f, 11.0f, 0.0f);
	
	btVector3 camoff(10,0,0);
	btMatrix3x3 mat;
	mat.setEulerYPR(m_cameraTimer * 0.01f, 0.0f, 0.0f);
	camoff = mat * camoff;
	
	btVector3 camera = lookat + camoff;
	
	m_camera.SetPos(camera);
	
	btVector3 forward = lookat - camera;
	forward.normalize();
	btVector3 up(0,1,0);
	
	btVector3 side = up.cross(forward) * 2.5f;
	
	
	m_camera.SetLookAt(lookat + side);
	
	m_scoreControl.NextFrame(delta);
	
	m_logo.NextFrame(delta);
	m_courseMedallion.NextFrame(delta);
	m_courseSplash.NextFrame(delta);
	m_startText.NextFrame(delta);
	m_practiceText.NextFrame(delta);
	m_copyrightText.NextFrame(delta);
	m_backText.NextFrame(delta);
	m_goText.NextFrame(delta);
	m_tournamentText.NextFrame(delta);
	
	m_scoreText.NextFrame(delta);
	m_scoreDesc.NextFrame(delta);
	
	m_courseNameText.NextFrame(delta);
	m_courseSubnameText.NextFrame(delta);
	m_courseDescText.NextFrame(delta);
	
	for(int i = 0; i < kNumCoursesPerScreen; i++)
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
		
	RGL.Enable(kBackfaceCull, true);
	RGL.Enable(kDepthTest, true);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);
	m_terrain.Render();
	
	m_flag.Render();
	
	RGL.Ortho(0.0f, 0.0f, 0.0f, 320.0f, 480.0f, 100.0f);
	RGL.LoadIdentity();
	RGL.Enable(kBackfaceCull, false);
	RGL.Enable(kDepthTest, false);
	
	
	if(m_state == kTitleScoreSummary || m_state == kTitleSplash)
	{
		m_scoreControl.Render();
		m_scoreText.Render();
		m_scoreDesc.Render();
	}
	
	m_logo.Render();
	
	m_courseMedallion.Render();
	m_courseSplash.Render();
	m_startText.Render();
	m_practiceText.Render();
	m_backText.Render();
	m_goText.Render();
	m_tournamentText.Render();
	
	m_courseNameText.Render();
	m_courseSubnameText.Render();
	m_courseDescText.Render();
	
	for(int i = 0; i < kNumCoursesPerScreen; i++)
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
			m_practiceText.TouchDown(rbt);
			break;
		case kTitleCourseSelect:
			for(int i = 0; i < kNumCoursesPerScreen; i++)
			{
				m_courseButtons[i].TouchDown(rbt);
			}
			m_backText.TouchDown(rbt);
			break;
		case kTitleGameOptions:
			m_goText.TouchDown(rbt);
			m_backText.TouchDown(rbt);
			break;
		case kTitlePracticeOptions:
			m_goText.TouchDown(rbt);
			m_backText.TouchDown(rbt);
			break;
	}
}

void RBUITitle::TouchMove(RudeTouch *rbt)
{
}

void RBUITitle::TouchUp(RudeTouch *rbt)
{
	eSoundEffect sfx = kSoundNone;
	
	switch(m_state)
	{
		case kTitleScoreSummary:
			sfx = kSoundUISelect;
			SetState(kTitleSplash);
			break;
		case kTitleSplash:
			if(m_startText.TouchUp(rbt))
			{
				SetState(kTitleCourseSelect);
				sfx = kSoundUISelect;
			}
			if(m_practiceText.TouchUp(rbt))
			{
				// special identifier for practice course
				m_course = kNumCourses - 1;
				SetState(kTitlePracticeOptions);
				sfx = kSoundUISelect;
			}
			break;
		case kTitleCourseSelect:
			for(int i = 0; i < kNumCoursesPerScreen; i++)
			{
				if(m_courseButtons[i].TouchUp(rbt))
				{
					if(RBTourTracker::Unlocked(i))
					{
						m_course = i;
						SetState(kTitleGameOptions);
						sfx = kSoundUISelect;
					}
					break;
				}
			}
			
			if(m_backText.TouchUp(rbt))
			{
				SetState(kTitleSplash);
				sfx = kSoundUIBack;
			}
			break;
		case kTitleGameOptions:
			if(m_goText.TouchUp(rbt))
			{
				SetState(kTitleReadyToPlay);
				sfx = kSoundUIStart;
			}
			if(m_backText.TouchUp(rbt))
			{
				SetState(kTitleCourseSelect);
				sfx = kSoundUIBack;
			}
			break;
		case kTitlePracticeOptions:
			if(m_goText.TouchUp(rbt))
			{
				SetState(kTitleReadyToPractice);
				sfx = kSoundUIStart;
			}
			if(m_backText.TouchUp(rbt))
			{
				SetState(kTitleSplash);
				sfx = kSoundUIBack;
			}
			break;
	}
	
	RudeSound::GetInstance()->PlayWave(sfx);
}

