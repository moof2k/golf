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
#include "RudeTweaker.h"

#include "RudeSound.h"


bool gDebugPerfectScore = false;
RUDE_TWEAK(DebugPerfectScore, kBool, gDebugPerfectScore);

const btVector3 kFlagPosition(-5.741f,-18.27f,-1572.807f);

const char * kMusicOnText = "Music: On";
const char * kMusicOffText = "Music: Off";

RBUITitle::RBUITitle()
{
	m_flagOffset = 0.0f;
	
	m_startedMusic = false;
	
	m_terrain.LoadMesh("par5_A");
	m_decorator.Load("par5_A");
	m_skybox.Load("skybox");
	
	m_flag.LoadMesh("sexy");
	m_flag.SetPosition(kFlagPosition);

	m_ui.Load("title_iphone");
	
	m_logo = m_ui.GetChildControl<RudeButtonControl>("logo");
	m_rangeMedallion = m_ui.GetChildControl<RudeButtonControl>("rangeMedallion");
	m_courseMedallion = m_ui.GetChildControl<RudeButtonControl>("courseMedallion");
	m_courseSplash = m_ui.GetChildControl<RudeButtonControl>("courseSplash");

	
	m_scoreControl.SetAnimType(kAnimPopSlide);
	m_scoreControl.SetRect(RudeRect(0,0,480,320));
	m_scoreControl.SetActiveHole(0, kCourseAll18);
	
	m_credits.SetAnimType(kAnimPopSlide);
	

	const int kCourseButtonTop = 66;
	const int kCourseButtonHeight = 58;
	const float kCourseAnimSpeedBase = 3.0f;
	const float kCourseAnimSpeedMod = -0.2f;
	for(int i = 0; i < kNumCourses; i++)
	{
		int offset = kCourseButtonTop + kCourseButtonHeight * i;
		
		m_courseButtons[i].SetAnimType(kAnimPopSlide);
		m_courseButtons[i].SetAnimSpeed(float(i) * kCourseAnimSpeedMod + kCourseAnimSpeedBase);
		m_courseButtons[i].SetTexture("coursebg");
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
	
	m_startText = m_ui.GetChildControl<RudeTextControl>("startText");
	m_practiceText = m_ui.GetChildControl<RudeTextControl>("practiceText");
	m_settingsText = m_ui.GetChildControl<RudeTextControl>("settingsText");
	m_goText = m_ui.GetChildControl<RudeTextControl>("goText");
	m_backText = m_ui.GetChildControl<RudeTextControl>("backText");
	
	m_copyrightText = m_ui.GetChildControl<RudeTextControl>("copyrightText");
	m_tournamentText = m_ui.GetChildControl<RudeTextControl>("tournamentText");
	m_courseNameText = m_ui.GetChildControl<RudeTextControl>("courseNameText");
	m_courseSubnameText = m_ui.GetChildControl<RudeTextControl>("courseSubnameText");
	m_courseDescText = m_ui.GetChildControl<RudeTextControl>("courseDescText");
	m_scoreText = m_ui.GetChildControl<RudeTextControl>("scoreText");
	m_scoreDesc = m_ui.GetChildControl<RudeTextControl>("scoreDesc");
	m_settingsTitleText = m_ui.GetChildControl<RudeTextControl>("settingsTitleText");
	m_musicToggle = m_ui.GetChildControl<RudeTextControl>("musicToggle");
	
	
	if(RudeSound::GetInstance()->GetMusicOn())
		m_musicToggle->SetText(kMusicOnText);
	else
		m_musicToggle->SetText(kMusicOffText);
	
	
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
			
			m_flagOffset = 0.0f;
			
			RefreshScores();
			
			m_logo->SetTranslation(btVector3(0,0,0));
			m_startText->SetTranslation(btVector3(0,0,0));
			m_practiceText->SetTranslation(btVector3(0,0,0));
			m_settingsText->SetTranslation(btVector3(0,0,0));

			m_settingsTitleText->SetTranslation(btVector3(400,0,0));
			m_settingsTitleText->SetDesiredTranslation(btVector3(400,0,0));
			m_musicToggle->SetTranslation(btVector3(400,0,0));
			m_musicToggle->SetDesiredTranslation(btVector3(400,0,0));
			
			
			m_credits.SetTranslation(btVector3(400,0,0));
			m_credits.SetDesiredTranslation(btVector3(400,0,0));
			
			m_scoreControl.SetTranslation(btVector3(-400,0,0));
			m_scoreControl.SetDesiredTranslation(btVector3(-400,0,0));
			
			m_courseMedallion->SetTranslation(btVector3(400,0,0));
			m_courseMedallion->SetDesiredTranslation(btVector3(400,0,0));
			
			m_courseSplash->SetTranslation(btVector3(400,0,0));
			m_courseSplash->SetDesiredTranslation(btVector3(400,0,0));
			
			m_rangeMedallion->SetTranslation(btVector3(400,0,0));
			m_rangeMedallion->SetDesiredTranslation(btVector3(400,0,0));
			
			m_backText->SetTranslation(btVector3(400,0,0));
			m_backText->SetDesiredTranslation(btVector3(400,0,0));
			
			m_tournamentText->SetTranslation(btVector3(400,0,0));
			m_tournamentText->SetDesiredTranslation(btVector3(400,0,0));
			
			for(int i = 0; i < kNumCourses; i++)
			{
				m_courseButtons[i].SetTranslation(btVector3(400,0,0));
				m_courseButtons[i].SetDesiredTranslation(btVector3(400,0,0));
			}
			
			m_courseNameText->SetTranslation(btVector3(400,0,0));
			m_courseSubnameText->SetTranslation(btVector3(400,0,0));
			m_courseDescText->SetTranslation(btVector3(400,0,0));
			m_courseNameText->SetDesiredTranslation(btVector3(400,0,0));
			m_courseSubnameText->SetDesiredTranslation(btVector3(400,0,0));
			m_courseDescText->SetDesiredTranslation(btVector3(400,0,0));
			
			m_goText->SetTranslation(btVector3(400,0,0));
			m_goText->SetDesiredTranslation(btVector3(400,0,0));
			
			m_scoreText->SetDesiredTranslation(btVector3(-400,0,0));
			m_scoreDesc->SetDesiredTranslation(btVector3(-400,0,0));
			m_scoreText->SetTranslation(btVector3(-400,0,0));
			m_scoreDesc->SetTranslation(btVector3(-400,0,0));
						
			break;
		case kTitleScoreSummary:
			RefreshScores();
			
			m_scoreControl.SetDesiredTranslation(btVector3(-400,0,0));
			
			m_courseNameText->SetDesiredTranslation(btVector3(-400,0,0));
			m_courseSubnameText->SetDesiredTranslation(btVector3(-400,0,0));
			m_courseDescText->SetDesiredTranslation(btVector3(-400,0,0));
			
			m_scoreText->SetDesiredTranslation(btVector3(-400,0,0));
			m_scoreDesc->SetDesiredTranslation(btVector3(-400,0,0));
			
			m_courseMedallion->SetDesiredTranslation(btVector3(-400,0,0));
			m_courseSplash->SetDesiredTranslation(btVector3(-400,0,0));
			
			break;
		case kTitleSplash:
			RefreshScores();
			
			m_logo->SetDesiredTranslation(btVector3(-400,0,0));
			m_startText->SetDesiredTranslation(btVector3(-400,0,0));
			m_practiceText->SetDesiredTranslation(btVector3(-400,0,0));
			m_settingsText->SetDesiredTranslation(btVector3(-400,0,0));
			m_copyrightText->SetDesiredTranslation(btVector3(0,100,0));
			
			break;
		case kTitleSettings:
			
			m_settingsTitleText->SetDesiredTranslation(btVector3(400,0,0));
			m_musicToggle->SetDesiredTranslation(btVector3(400,0,0));
			
			m_credits.SetDesiredTranslation(btVector3(400,0,0));
			m_backText->SetDesiredTranslation(btVector3(400,0,0));
			
			break;
		case kTitleCourseSelect:
		{
			
			float dir = 1.0f;
			if(state == kTitleGameOptions)
			{
				dir = -1.0f;
			}
			else
				m_backText->SetDesiredTranslation(btVector3(dir * 400,0,0));
			
			for(int i = 0; i < kNumCourses; i++)
			{
				m_courseButtons[i].SetDesiredTranslation(btVector3(dir * 400,0,0));
			}
			
			m_tournamentText->SetDesiredTranslation(btVector3(dir * 400,0,0));
		}
			break;
		case kTitleGameOptions:
			if(state != kTitleReadyToPlay)
			{
				m_goText->SetDesiredTranslation(btVector3(400,0,0));
				
				m_courseNameText->SetDesiredTranslation(btVector3(400,0,0));
				m_courseSubnameText->SetDesiredTranslation(btVector3(400,0,0));
				m_courseDescText->SetDesiredTranslation(btVector3(400,0,0));
				
				m_courseMedallion->SetDesiredTranslation(btVector3(400,0,0));
				m_courseSplash->SetDesiredTranslation(btVector3(400,0,0));
			}
			
			break;
			
		case kTitlePracticeOptions:
			if(state != kTitleReadyToPractice)
			{
				m_courseNameText->SetDesiredTranslation(btVector3(400,0,0));
				
				m_rangeMedallion->SetDesiredTranslation(btVector3(400,0,0));
			}
			
			m_goText->SetDesiredTranslation(btVector3(400,0,0));
			m_backText->SetDesiredTranslation(btVector3(400,0,0));
			break;
	}
	
	m_state = state;
	
	switch(m_state)
	{
		case kTitleScoreSummary:
			{
				bool completed = RBTourTracker::Completed(m_course);
				
				if(completed)
				{
					m_courseSplash->SetTexture(m_courseButtons[m_course].GetImageStr());
					
					m_courseMedallion->SetTranslation(btVector3(0,0,0));
					m_courseSplash->SetTranslation(btVector3(0,0,0));
					
					m_courseMedallion->SetDesiredTranslation(btVector3(0,0,0));
					m_courseSplash->SetDesiredTranslation(btVector3(0,0,0));
				}
				else
				{
					m_courseMedallion->SetTranslation(btVector3(-400,0,0));
					m_courseSplash->SetTranslation(btVector3(-400,0,0));
					
					m_courseMedallion->SetDesiredTranslation(btVector3(-400,0,0));
					m_courseSplash->SetDesiredTranslation(btVector3(-400,0,0));
				}
				
				m_logo->SetTranslation(btVector3(400,0,0));
				m_startText->SetTranslation(btVector3(400,0,0));
				m_practiceText->SetTranslation(btVector3(400,0,0));
				m_settingsText->SetTranslation(btVector3(400,0,0));
				
				m_logo->SetDesiredTranslation(btVector3(400,0,0));
				m_startText->SetDesiredTranslation(btVector3(400,0,0));
				m_practiceText->SetDesiredTranslation(btVector3(400,0,0));
				m_settingsText->SetDesiredTranslation(btVector3(400,0,0));
				
				m_scoreControl.SetActiveHole(-1, m_courseButtons[m_course].m_holes);
				m_scoreControl.SetDesiredTranslation(btVector3(0,0,0));
				
				m_courseNameText->SetTranslation(btVector3(-400,0,0));
				m_courseSubnameText->SetTranslation(btVector3(-400,0,0));
				m_courseDescText->SetTranslation(btVector3(-400,0,0));
				m_courseNameText->SetDesiredTranslation(btVector3(0,0,0));
				m_courseSubnameText->SetDesiredTranslation(btVector3(0,0,0));
				m_courseDescText->SetDesiredTranslation(btVector3(0,0,0));
				
				RUDE_ASSERT(m_course >= 0, "Invalid course");
				m_courseNameText->SetText(m_courseButtons[m_course].GetNameStr());
				m_courseSubnameText->SetText(m_courseButtons[m_course].GetSubnameStr());
				m_courseDescText->SetText(m_courseButtons[m_course].GetDescStr());
				
				m_scoreText->SetDesiredTranslation(btVector3(0,0,0));
				m_scoreDesc->SetDesiredTranslation(btVector3(0,0,0));
			}
			break;
			
		case kTitleSplash:
			
			m_logo->SetDesiredTranslation(btVector3(0,0,0));
			m_startText->SetDesiredTranslation(btVector3(0,0,0));
			m_practiceText->SetDesiredTranslation(btVector3(0,0,0));
			m_copyrightText->SetDesiredTranslation(btVector3(0,0,0));
			m_settingsText->SetDesiredTranslation(btVector3(0,0,0));
			break;
			
		case kTitleSettings:
			m_settingsTitleText->SetDesiredTranslation(btVector3(0,0,0));
			m_musicToggle->SetDesiredTranslation(btVector3(0,0,0));
			
			m_backText->SetDesiredTranslation(btVector3(0,0,0));
			m_credits.SetDesiredTranslation(btVector3(0,0,0));
			break;
			
		case kTitleCourseSelect:
			
			m_tournamentText->SetDesiredTranslation(btVector3(0,0,0));
			
			m_backText->SetDesiredTranslation(btVector3(0,0,0));
			for(int i = 0; i < kNumCourses; i++)
			{
				m_courseButtons[i].SetDesiredTranslation(btVector3(0,0,0));
			}
			break;
			
		case kTitleGameOptions:
			m_goText->SetDesiredTranslation(btVector3(0,0,0));
			
			m_courseMedallion->SetTranslation(btVector3(400,0,0));
			m_courseSplash->SetTranslation(btVector3(400,0,0));
			m_courseMedallion->SetDesiredTranslation(btVector3(400,0,0));
			m_courseSplash->SetDesiredTranslation(btVector3(400,0,0));
			
			m_courseNameText->SetTranslation(btVector3(400,0,0));
			m_courseSubnameText->SetTranslation(btVector3(400,0,0));
			m_courseDescText->SetTranslation(btVector3(400,0,0));
			m_courseNameText->SetDesiredTranslation(btVector3(0,0,0));
			m_courseSubnameText->SetDesiredTranslation(btVector3(0,0,0));
			m_courseDescText->SetDesiredTranslation(btVector3(0,0,0));
			
			RUDE_ASSERT(m_course >= 0, "Invalid course");
			m_courseNameText->SetText(m_courseButtons[m_course].GetNameStr());
			m_courseSubnameText->SetText(m_courseButtons[m_course].GetSubnameStr());
			m_courseDescText->SetText(m_courseButtons[m_course].GetDescStr());
			
			m_courseSplash->SetTexture(m_courseButtons[m_course].GetImageStr());
			
			m_courseMedallion->SetDesiredTranslation(btVector3(0,0,0));
			m_courseSplash->SetDesiredTranslation(btVector3(0,0,0));
			
			break;
			
		case kTitlePracticeOptions:
			m_goText->SetDesiredTranslation(btVector3(0,0,0));
			m_backText->SetDesiredTranslation(btVector3(0,0,0));
			
			m_courseNameText->SetText(sCourseData[kNumCourses - 1].m_name);
			
			m_courseNameText->SetDesiredTranslation(btVector3(0,0,0));
			
			m_rangeMedallion->SetDesiredTranslation(btVector3(0,0,0));
			
			break;
			
		case kTitleReadyToPlay:
			RudeSound::GetInstance()->BgmVolFade(-0.5f);
			
			m_readyTimer = 0.0f;
			m_goText->SetDesiredTranslation(btVector3(0,100,0));
			m_backText->SetDesiredTranslation(btVector3(0,100,0));
			break;
			
		case kTitleReadyToPractice:
			RudeSound::GetInstance()->BgmVolFade(-0.5f);
			
			m_readyTimer = 0.0f;
			m_goText->SetDesiredTranslation(btVector3(0,100,0));
			m_backText->SetDesiredTranslation(btVector3(0,100,0));
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
				sprintf(str, " ±0");
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
			m_courseButtons[i].SetTexture("coursebg");
			
			if(RBTourTracker::Completed(i))
				m_courseButtons[i].m_completed = true;
			else
				m_courseButtons[i].m_completed = false;
		}
		else
		{
			m_courseButtons[i].SetTexture("coursebg_locked");
			
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
	
	m_scoreText->SetText(str);
	
	RBCourseEntry *course = GetCourseData(m_course);
	
	int scoreoffset = score - course->m_completionScore;
	
	if (scoreoffset > 60)
		m_scoreDesc->SetText("FWIW, +81 is the worst you can do on a 9 Hole");
	else if(scoreoffset > 20)
		m_scoreDesc->SetText("Are we a little distracted?");
	else if(scoreoffset > 5)
		m_scoreDesc->SetText("You can do better!");
	else if(scoreoffset > 0)
		m_scoreDesc->SetText("Try harder next time!");
	else if(scoreoffset > -3)
		m_scoreDesc->SetText("Good game!");
	else if(scoreoffset > -6)
		m_scoreDesc->SetText("Great game!");
	else
		m_scoreDesc->SetText("You have the Magic Touch!");
	
}

void RBUITitle::NextFrame(float delta)
{
	if(gDebugPerfectScore)
	{
		for(int i = 0; i < kNumCourses; i++)
		{
			RBTourTracker::SetScore(i, 0);
		}
		gDebugPerfectScore = false;
	}
	
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
	m_credits.NextFrame(delta);

	m_ui.NextFrame(delta);
	
	for(int i = 0; i < kNumCoursesPerScreen; i++)
	{
		m_courseButtons[i].NextFrame(delta);
	}
}

void RBUITitle::Render(float width, float height)
{
	RGL.SetViewport(0, 0, (int) height, (int) width);
	
	float aspect = width / height;
	m_camera.SetView(aspect);
	RGL.LoadIdentity();
	
	m_skybox.Render();
		
	RGL.Enable(kBackfaceCull, true);
	RGL.Enable(kDepthTest, true);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);
	m_terrain.Render();
	
	m_decorator.Render();
	
	m_flag.Render();
	
	RGL.Ortho(0.0f, 0.0f, 0.0f, width, height, 100.0f);
	RGL.LoadIdentity();
	RGL.Enable(kBackfaceCull, false);
	RGL.Enable(kDepthTest, false);
	
	
	m_ui.Render();
	
	
	for(int i = 0; i < kNumCoursesPerScreen; i++)
	{
		m_courseButtons[i].Render();
	}
	
	if(m_state == kTitleScoreSummary || m_state == kTitleSplash)
	{
		m_scoreControl.Render();
	}
	
	if(m_state == kTitleSplash || m_state == kTitleSettings)
	{
		m_credits.Render();
	}
	
}

void RBUITitle::TouchDown(RudeTouch *rbt)
{
	switch(m_state)
	{
		case kTitleSplash:
			m_startText->TouchDown(rbt);
			m_practiceText->TouchDown(rbt);
			m_settingsText->TouchDown(rbt);
			break;
		case kTitleSettings:
			m_backText->TouchDown(rbt);
			m_musicToggle->TouchDown(rbt);
			break;
		case kTitleCourseSelect:
			for(int i = 0; i < kNumCoursesPerScreen; i++)
			{
				m_courseButtons[i].TouchDown(rbt);
			}
			m_backText->TouchDown(rbt);
			break;
		case kTitleGameOptions:
			m_goText->TouchDown(rbt);
			m_backText->TouchDown(rbt);
			break;
		case kTitlePracticeOptions:
			m_goText->TouchDown(rbt);
			m_backText->TouchDown(rbt);
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
			if(m_startText->TouchUp(rbt))
			{
				SetState(kTitleCourseSelect);
				sfx = kSoundUISelect;
			}
			if(m_practiceText->TouchUp(rbt))
			{
				// special identifier for practice course
				m_course = kNumCourses - 1;
				SetState(kTitlePracticeOptions);
				sfx = kSoundUISelect;
			}
			if(m_settingsText->TouchUp(rbt))
			{
				SetState(kTitleSettings);
				sfx = kSoundUISelect;
			}
			break;
		case kTitleSettings:
			if(m_backText->TouchUp(rbt))
			{
				SetState(kTitleSplash);
				sfx = kSoundUIBack;
			}
			if(m_musicToggle->TouchUp(rbt))
			{
				bool musicon = RudeSound::GetInstance()->ToggleMusic();
				
				if(musicon)
				{
					m_startedMusic = false;
					m_musicToggle->SetText(kMusicOnText);
				}
				else
				{
					m_musicToggle->SetText(kMusicOffText);
					m_startedMusic = true;
				}
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
			
			if(m_backText->TouchUp(rbt))
			{
				SetState(kTitleSplash);
				sfx = kSoundUIBack;
			}
			break;
		case kTitleGameOptions:
			if(m_goText->TouchUp(rbt))
			{
				SetState(kTitleReadyToPlay);
				sfx = kSoundUIStart;
			}
			if(m_backText->TouchUp(rbt))
			{
				SetState(kTitleCourseSelect);
				sfx = kSoundUIBack;
			}
			break;
		case kTitlePracticeOptions:
			if(m_goText->TouchUp(rbt))
			{
				SetState(kTitleReadyToPractice);
				sfx = kSoundUIStart;
			}
			if(m_backText->TouchUp(rbt))
			{
				SetState(kTitleSplash);
				sfx = kSoundUIBack;
			}
			break;
	}
	
	RudeSound::GetInstance()->PlayWave(sfx);
}

