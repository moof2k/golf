/*
 *  RBGame.cpp
 *  golf
 *
 *  Created by Robert Rose on 10/10/08.
 *  Copyright 2008 Bork 3D LLC. All rights reserved.
 *
 */

#include "RBGame.h"
#include "RudeMath.h"
#include "RudeText.h"
#include "RudeTimeCounter.h"
#include "RudePerf.h"
#include "RudeRegistry.h"
#include "RudeSound.h"
#include "RudeTimer.h"
#include "RudeTweaker.h"

#include "RBUITitle.h"

bool gDebugDisplayFPS = true;
RUDE_TWEAK(DebugDisplayFPS, kBool, gDebugDisplayFPS);

RBGame::RBGame()
{
	RudeMath::EnableFPE();
	
	srand(time(NULL));

	m_keymap = RBKeyMap::getInstance();

	m_game = new RBUITitle();

	Init();
}

RBGame::~RBGame()
{
	Destroy();
}

void RBGame::Destroy()
{

}

void RBGame::Init()
{
	Destroy();
}


void RBGame::Render(float delta, float width, float height)
{
	RudePerf::NextFrame(delta);

	RudeTimer timer;
	
	
	RudeSound::GetInstance()->Tick(delta);
	
	
	const float kMaxDelta = 0.5f;
	
	if(delta > kMaxDelta)
		delta = kMaxDelta;
	
	m_game->NextFrame(delta);
	m_game->Render(width, height);
	
	
	
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
		float d_fps = 1.0f / d_s;
		
		unsigned int color = 0xFF00FF00;
		
		if(d_fps < 14.0f)
			color = 0xFF0000FF;
		else if(d_fps < 17.0f)
			color = 0xFF00FFFF;
		
		RudePerf::PrintAll();
		
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

void RBGame::Pause()
{

}


