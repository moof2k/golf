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

	_state = GameRBT;

	_rbt = NULL;

	_keymap = RBKeyMap::getInstance();


	Init();
}

RBGame::~RBGame()
{
	Destroy();
}

void RBGame::Destroy()
{
	if(_rbt)
		delete _rbt;


}

void RBGame::Init()
{
	Destroy();

	_rbt = new RBTRound();

	_state = GameRBT; // GameTitle;

}


void RBGame::Render(float delta, float aspect)
{
	uint64_t starttime = mach_absolute_time();
	
	switch(_state)
	{


		case GameRBT:
			_rbt->NextFrame(delta);
			_rbt->Render(aspect);

			if(_rbt->Done())
				m_done = true;
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
	RBKey rbk = _keymap->mapKey(k);

	switch(_state)
	{
		case GameRBT:
			//_rbt->KeyDown(rbk);
			break;

	}
}

void RBGame::KeyUp(RudeKey k)
{
	RBKey rbk = _keymap->mapKey(k);

	switch(_state)
	{

		case GameRBT:
			//_rbt->KeyUp(rbk);
			break;

	}
}

void RBGame::StylusDown(RudeScreenVertex &p)
{
	switch(_state)
	{

			
		case GameRBT:
			//_rbt->StylusDown(p);
			break;
			
	}
}

void RBGame::StylusUp(RudeScreenVertex &p)
{
	switch(_state)
	{

			
		case GameRBT:
			//_rbt->StylusUp(p);
			break;
			
	}
}

void RBGame::StylusMove(RudeScreenVertex &p)
{
	switch(_state)
	{

			
		case GameRBT:
			//_rbt->StylusMove(p);
			break;
			
	}
}

void RBGame::TouchDown(RudeScreenVertex &n)
{
	RudeTouch *touch = _touchtracker.NewTouch(n);
	RUDE_ASSERT(touch, "Could not create touch");
	
	switch(_state)
	{
		case GameRBT:
			_rbt->TouchDown(touch);
			break;
	}
}

void RBGame::TouchMove(RudeScreenVertex &n, RudeScreenVertex &p)
{
	RudeTouch *touch = _touchtracker.GetTouch(p);
	touch->m_location = n;
	
	switch(_state)
	{
		case GameRBT:
			_rbt->TouchMove(touch);
			break;
	}
}

void RBGame::TouchUp(RudeScreenVertex &n, RudeScreenVertex &p)
{
	// p is the previous position from movement (so it might not have changed..)
	RudeTouch *touch = _touchtracker.GetTouch(n);
	touch->m_location = n;
	
	switch(_state)
	{
		case GameRBT:
			_rbt->TouchUp(touch);
			break;
	}
	
	_touchtracker.ReleaseTouch(touch);
}

void RBGame::Pause()
{

}


