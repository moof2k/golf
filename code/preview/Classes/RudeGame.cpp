#include "RudeGame.h"



#ifdef RUDE_WIN
#include <stdio.h>
#include <Wtypes.h>
#endif

RudeGame::RudeGame()
{
#ifdef RUDE_PALMOS
	_prevtime = 0;
#endif

#ifdef RUDE_SYMBIAN
	_prevtime.HomeTime();
#endif

#ifdef RUDE_WIN32
	_prevtime = 0.0f;
#endif

	m_done = false;

	for(_rti = 0; _rti < REC_FRAMES; _rti++)
		_rendertimes[_rti] = 0.0f;
		
	_rotate = 45.0f;
	
}

RudeGame::~RudeGame()
{
	
}

void RudeGame::NextFrame(float delta)
{
}

void RudeGame::Render(float aspect)
{
}


void RudeGame::KeyDown(RudeKey k)
{
}

void RudeGame::KeyUp(RudeKey k)
{
}

void RudeGame::StylusDown(RudeScreenVertex &p)
{
	m_done = true;
}

void RudeGame::StylusMove(RudeScreenVertex &p)
{

}

void RudeGame::StylusUp(RudeScreenVertex &p)
{

}

void RudeGame::TouchDown(RudeScreenVertex &n) {}
void RudeGame::TouchMove(RudeScreenVertex &n, RudeScreenVertex &p) {}
void RudeGame::TouchUp(RudeScreenVertex &n, RudeScreenVertex &p) {}

void RudeGame::Pause()
{

}

float RudeGame::GetTime()
{
#ifdef RUDE_WIN

#ifndef _WIN32_WCE
	SYSTEMTIME s;
	GetSystemTime(&s);

	float t = (float) (s.wSecond + s.wMinute * 60 + s.wHour * 3600);
	t += ((float) s.wMilliseconds / 1000.f);

	return t;
#else

	return ((float) GetTickCount()) * 0.001f;
#endif

#endif

	return 0.0f;
}

float RudeGame::GetDelta()
{
#ifdef RUDE_PALM

	static UInt16 tps = SysTicksPerSecond();
	UInt32 ticks = TimGetTicks();
	
	float delta = (float) (ticks - _prevtime) / ((float) tps);
	_prevtime = ticks;
	
	//if(delta > 0.3f)
	//	delta = 0.3f;
	
	return delta;
#endif

#ifdef RUDE_WIN
	
	float t = GetTime();

	float delta = t - _prevtime;

	if(delta > 0.3f)
		delta = 0.3f;

	_prevtime = t;

	return delta;
#endif

#ifdef RUDE_SYMBIAN

	TTime thistime;
	thistime.HomeTime();
	
	TTimeIntervalMicroSeconds deltam = thistime.MicroSecondsFrom(_prevtime);
	
	_prevtime = thistime;

	TInt64 deltai = deltam.Int64();
	TUint deltalow = deltai.Low();
	
	float delta = ((float) deltalow) * 0.000001f;
	
	return delta;
	
#endif
	
#ifdef RUDE_IPHONE
	return 0.0f;
#endif

}



