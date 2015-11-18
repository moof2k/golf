/*
 *  RBGame.cpp
 *  golf
 *
 *  Created by Robert Rose on 10/10/08.
 *  Copyright 2008 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef STRICT
#define STRICT
#endif

#include "RBGame.h"
#include "RudeMath.h"
#include "RudeText.h"
#include "RudeTimeCounter.h"
#include "RudePerf.h"
#include "RudeRegistry.h"
#include "RudeSound.h"
#include "RudeTimer.h"
#include "RudeTweaker.h"

bool gDebugDisplayFPS = true;
RUDE_TWEAK(DebugDisplayFPS, kBool, gDebugDisplayFPS);

RBGame::RBGame()
{
	RudeMath::EnableFPE();
	
	srand((unsigned int) time(NULL));

	m_state = kGameNone;
	m_game = 0;
	m_course = 0;

	m_tutorial = NULL;
	m_uiTitle = NULL;
	m_rbt = NULL;

	m_keymap = RBKeyMap::getInstance();

	RBTourTracker::Load();

	Init();
}

RBGame::~RBGame()
{
	Destroy();
}

void RBGame::Destroy()
{
	if(m_rbt)
		delete m_rbt;

	m_game = 0;
}

void RBGame::Init()
{
	Destroy();

	m_uiTitle = new RBUITitle();
	m_rbt = new RBTRound();
	m_tutorial = new RBUITutorial();
	
	if(LoadState() != 0)
		SetState(kGameTitle);
}

void RBGame::SaveState()
{
	RudeRegistry *reg = RudeRegistry::GetSingleton();
	
	tRBGameSaveState save;
	save.state = m_state;
	save.course = m_course;
	
	reg->SetByte("GOLF", "GS_GAME_STATE", &save, sizeof(save));
}

int RBGame::LoadState()
{
	RudeRegistry *reg = RudeRegistry::GetSingleton();
	
	tRBGameSaveState load;
	int loadsize = sizeof(load);
	if(reg->QueryByte("GOLF", "GS_GAME_STATE", &load, &loadsize) == 0)
	{
		m_course = load.course;
		
		if(load.state == kGameTutorial)
			load.state = kGameTitle;
		
		SetState(load.state);
		
		if(load.state == kGameRBT)
		{
			m_rbt->SetCourse(m_course);
			
			if(m_rbt->LoadState() != 0)
			{
				SetState(kGameTitle);
				return -1;
			}
		}
	}
	else
		return -1;
	
	return 0;
}

void RBGame::SetState(eGameState state)
{
	eGameState prevstate = m_state;
	
	m_state = state;
	
	switch(m_state)
	{
		case kGameTitle:
			m_uiTitle->Reset();
			if(prevstate == kGameRBT)
			{
				eRBTRoundResult result = m_rbt->GetResult();
				
				if(result == kRoundComplete)
				{
					int course = m_rbt->GetCourse();
					
					RBCourseEntry *coursedata = GetCourseData(course);
					RUDE_ASSERT(coursedata, "Invalid course data");
					
					RBScoreTracker *tracker = GetScoreTracker(0);
					int score = tracker->GetTotalScore();
					
					RBTourTracker::SetScore(course, score);
					
					m_uiTitle->SetState(kTitleScoreSummary);
					m_uiTitle->SetCourseSelection(course);
					m_uiTitle->SetCourseScore(score);
				}
				else
				{
					m_uiTitle->SetState(kTitleSplash);
				}
			}
			m_game = m_uiTitle;
			break;
		case kGameRBT:
		{
			int course = m_uiTitle->GetCourseSelection();
			m_rbt->Reset();
			m_rbt->SetCourse(course);
			m_game = m_rbt;
		}
			break;
		case kGameTutorial:
		{
			int course = m_uiTitle->GetCourseSelection();
			m_tutorial->Reset();
			m_tutorial->SetCourse(course);
			m_game = m_tutorial;
		}
			break;
            
        default:
            break;
	}
	
	SaveState();
}


void RBGame::Render(float delta, float width, float height, int camera)
{
	RudePerf::NextFrame(delta);

	RudeTimer timer;
	
	
	RudeSound::GetInstance()->Tick(delta);
	
	RUDE_ASSERT(m_game, "No state set");
	
	const float kMaxDelta = 0.5f;
	
	if(delta > kMaxDelta)
		delta = kMaxDelta;
	
	// Only increment game step on left camera (camera 0)
	if(camera == 0)
		m_game->NextFrame(delta);

	m_game->Render(width, height, camera);
	
	switch(m_state)
	{
		case kGameTitle:
			
			if(m_uiTitle->Done())
			{
				if(RUDE_IPAD)
					SetState(kGameRBT);
				else
					SetState(kGameTutorial);
			}
			break;
		case kGameRBT:

			if(m_rbt->Done())
				SetState(kGameTitle);
			break;
			
		case kGameTutorial:
			
			if(m_tutorial->Done())
				SetState(kGameRBT);
			break;

	}
	
	float actualElapsedSeconds = timer.ElapsedSeconds();
	RudePerf::AddStat(kFrameTotal, actualElapsedSeconds * 1000.0f);

	if(gDebugDisplayFPS)
	{
		
		
		static RudeTimeCounter sActualTimes;
		sActualTimes.AddTime(actualElapsedSeconds);
		
		static RudeTimeCounter sDeltaTimes;
		sDeltaTimes.AddTime(delta);
		
		float a_s = sActualTimes.GetAverage();
		float a_ms = a_s * 1000.0f;
		
		float d_s = sDeltaTimes.GetAverage();
		float d_ms = d_s * 1000.0f;

		float d_fps = 0.0f;
		if(d_s > 0.0f)
			d_fps = 1.0f / d_s;
		
		unsigned int color = 0xFF00FF00;
		
		if(d_fps < 14.0f)
			color = 0xFF0000FF;
		else if(d_fps < 17.0f)
			color = 0xFF00FFFF;
		
		RudePerf::PrintAll();
		
		if(RUDE_IPAD)
			RudeText::Print(0.0f, 0.0f, 0.15f, color, "FPS: %.1f CPU: %.1fms ACPU: %.1fms", d_fps, d_ms, a_ms);
		else
			RudeText::Print(0.0f, 0.0f, 0.5f, color, "FPS: %.1f CPU: %.1fms ACPU: %.1fms", d_fps, d_ms, a_ms);

		RudeText::Flush();
	
	}

}


void RBGame::KeyDown(RudeKey k)
{
	//RBKey rbk = _keymap->mapKey(k);

}

void RBGame::KeyUp(RudeKey k)
{
	//RBKey rbk = _keymap->mapKey(k);

}

void RBGame::StylusDown(RudeScreenVertex &p)
{

}

void RBGame::StylusUp(RudeScreenVertex &p)
{

}

void RBGame::StylusMove(RudeScreenVertex &p)
{

}

void RBGame::TouchDown(RudeScreenVertex &n)
{
	RudeTouch *touch = m_touchtracker.NewTouch(n);
	RUDE_ASSERT(touch, "Could not create touch");
	
	m_game->TouchDown(touch);
}

void RBGame::TouchMove(RudeScreenVertex &n, RudeScreenVertex &p)
{
	RudeTouch *touch = m_touchtracker.GetTouch(p);
	
	if(touch == 0)
		return;
	
	touch->m_location = n;
	
	m_game->TouchMove(touch);
}

void RBGame::TouchUp(RudeScreenVertex &n, RudeScreenVertex &p)
{	
	// p is the previous position from movement (so it might not have changed..)
	RudeTouch *touch = m_touchtracker.GetTouch(n);
	
	if(touch == 0)
		touch = m_touchtracker.GetTouch(p);
	
	if(touch == 0)
		return;
	
	touch->m_location = n;
	
	m_game->TouchUp(touch);
	
	m_touchtracker.ReleaseTouch(touch);
}

void RBGame::ScrollWheel(RudeScreenVertex &d)
{
    m_game->ScrollWheel(d);
}


void RBGame::Resize()
{
	m_game->Resize();
}

void RBGame::Pause()
{

}

void RBGame::OrientationChange()
{
	m_touchtracker.ReleaseAllTouches();
}
