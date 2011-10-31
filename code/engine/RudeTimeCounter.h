/*
 *  RudeTimeCounter.h
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef __H_RudeTimeCounter
#define __H_RudeTimeCounter

const int kNumTimes = 2;

class RudeTimeCounter {

public:
	RudeTimeCounter();
	
	void AddTime(float t)
	{
		m_times[m_curTime] = t;
		m_curTime++;
		if(m_curTime > kNumTimes)
			m_curTime = 0;
	}
	
	float GetAverage()
	{
		float r = 0.0f;
		for(int i = 0; i < kNumTimes; i++)
			r += m_times[i];
		r = r / ((float) kNumTimes);
		return r;
	}
	
private:
	
	float m_times[kNumTimes];
	int m_curTime;
};

#endif
