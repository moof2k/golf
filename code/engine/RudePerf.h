/*
 *  RudePerf.h
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef __H_RudePerf
#define __H_RudePerf

#include "Rude.h"
#include "RudeTimer.h"

//#define RUDE_PERF

#ifdef RUDE_PERF

#define RUDE_PERF_START(name) \
	RudeTimer rptimer_##name;

#define RUDE_PERF_STOP(name) \
	float rptimerdelta_##name = rptimer_##name.ElapsedMilliseconds(); \
	RudePerf::AddStat(name, rptimerdelta_##name);

#else

#define RUDE_PERF_START(name)  
#define RUDE_PERF_STOP(name)

#endif

typedef enum {
	kPerfRBTGameNextFrame,
	kPerfRBTGameRender1,
	kPerfRudeSkinMeshRender,
	kPerfRBTGameRender2,
	kPerfRBTGameRenderUI,
	kPerfTouchMove,
	kPerfFreshGuide,
	kPerfPhysics,
	kFrameTotal,
	
	kNumPerfs
	
} eRudePerfStat;

const int kNumPerfFrames = 15;

class RudePerf
{
public:
	
	static void NextFrame(float delta);
	
	static void AddStat(eRudePerfStat stat, float value)
	{
		m_perfs[stat][m_perfFrame] += value;
	}
	
	static float GetStat(eRudePerfStat stat);
	static void GetStatMinMaxAvg(eRudePerfStat stat, float &min, float &max, float &avg);
	
	static void PrintAll();
	
private:
	
	static int m_perfFrame;
	static float m_perfs[kNumPerfs][kNumPerfFrames];
	
};


#endif
