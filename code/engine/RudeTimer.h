/*
 *  RudeTimer.h
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef __H_RudeTimer
#define __H_RudeTimer

#include "Rude.h"

#ifdef RUDE_IPHONE
#include <mach/mach.h>
#include <mach/mach_time.h>
#include <unistd.h>
#endif


class RudeTimer
{
public:

	RudeTimer()
	{
		m_starttime = mach_absolute_time();
	}
	
	uint64_t ElapsedRaw()
	{
		return mach_absolute_time() - m_starttime;
	}
	
	uint64_t ElapsedNanoseconds()
	{
		static mach_timebase_info_data_t sTimebaseInfo = { 0,0 };
		if ( sTimebaseInfo.denom == 0 ) {
			(void) mach_timebase_info(&sTimebaseInfo);
		}
		
		return ElapsedRaw() * sTimebaseInfo.numer / sTimebaseInfo.denom;
	}
	
	float ElapsedMilliseconds()
	{
		return ((float) ElapsedNanoseconds()) / 1000000.0f;
	}
	
	float ElapsedSeconds()
	{
		return ((float) ElapsedNanoseconds()) / 1000000000.0f;
	}

private:
	
	uint64_t m_starttime;
	
};


#endif
