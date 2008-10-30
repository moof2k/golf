#ifndef STRICT
#define STRICT
#endif

#include "RBGame.h"
#include "RudeMath.h"
#include "RudeText.h"
#include "RudeTimeCounter.h"

#include <mach/mach.h>
#include <mach/mach_time.h>
#include <unistd.h>

RBGame::RBGame()
{
	RudeMath::EnableFPE();

	m_state = kGameRBT;
	m_game = 0;

	m_rbt = NULL;

	m_keymap = RBKeyMap::getInstance();


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

	SetState(kGameTitle);
}

void RBGame::SetState(eGameState state)
{
	m_state = state;
	
	switch(m_state)
	{
		case kGameTitle:
			m_uiTitle->Reset();
			m_game = m_uiTitle;
			break;
		case kGameRBT:
			m_rbt->Reset();
			m_game = m_rbt;
			break;
	}
}


void RBGame::Render(float delta, float aspect)
{
	uint64_t starttime = mach_absolute_time();
	
	RUDE_ASSERT(m_game, "No state set");
	
	const float kMaxDelta = 0.5f;
	
	if(delta > kMaxDelta)
		delta = kMaxDelta;
	
	m_game->NextFrame(delta);
	m_game->Render(aspect);
	
	switch(m_state)
	{
		case kGameTitle:
			
			if(m_uiTitle->Done())
				SetState(kGameRBT);
			break;
		case kGameRBT:

			if(m_rbt->Done())
				SetState(kGameTitle);
			break;

	}
	
	uint64_t endtime = mach_absolute_time();
	uint64_t actualtime = endtime - starttime;
	
	static mach_timebase_info_data_t    sTimebaseInfo = { 0,0 };
	if ( sTimebaseInfo.denom == 0 ) {
        (void) mach_timebase_info(&sTimebaseInfo);
    }
	
	uint64_t elapsedNano = actualtime * sTimebaseInfo.numer / sTimebaseInfo.denom;
	float actualElapsedSeconds = ((float) elapsedNano) / 1000000000.0f;
	
	static RudeTimeCounter sActualTimes;
	sActualTimes.AddTime(actualElapsedSeconds);
	
	static RudeTimeCounter sDeltaTimes;
	sDeltaTimes.AddTime(delta);
	
	float a_s = sActualTimes.GetAverage();
	float a_ms = a_s * 1000.0f;
	
	float d_s = sDeltaTimes.GetAverage();
	float d_ms = d_s * 1000.0f;
	float d_fps = 1.0f / d_s;
	
	RudeText::Print(0.0f, 0.0f, 0.5f, 0xFF00FF00, "FPS: %.1f CPU: %.1fms ACPU: %.1fms", d_fps, d_ms, a_ms);
	RudeText::Flush();

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
	touch->m_location = n;
	
	m_game->TouchUp(touch);
	
	m_touchtracker.ReleaseTouch(touch);
}

void RBGame::Pause()
{

}


