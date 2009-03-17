/*
 *  RudeTimeCounter.cpp
 *  golf
 *
 *  Created by Robert Rose on 9/3/08.
 *  Copyright 2008 Bork 3D LLC. All rights reserved.
 *
 */

#include "RudeTimeCounter.h"


RudeTimeCounter::RudeTimeCounter()
{
	m_curTime = 0;
	
	for(int i = 0; i < kNumTimes; i++)
		m_times[i] = 0.0f;
}

