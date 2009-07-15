/*
 *  RudeTimeCounter.cpp
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#include "RudeTimeCounter.h"


RudeTimeCounter::RudeTimeCounter()
{
	m_curTime = 0;
	
	for(int i = 0; i < kNumTimes; i++)
		m_times[i] = 0.0f;
}

