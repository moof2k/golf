/*
 *  RudePerf.cpp
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#include "RudePerf.h"
#include "RudeDebug.h"
#include "RudeText.h"

int RudePerf::m_perfFrame = 0;
float RudePerf::m_perfs[kNumPerfs][kNumPerfFrames];

const char * kStatNames[kNumPerfs] = {
	"RBTGame::NextFrame",
	"RBTGame::Render1",
	"SkinMesh::Render",
	"RBTGame::Render2",
	"RBTGame::RenderUI",
	"TouchMove",
	"FreshGuide",
	"Physics",
	"Frame Total",
};

void RudePerf::NextFrame(float delta)
{
	m_perfFrame += 1;
	
	if(m_perfFrame >= kNumPerfFrames)
		m_perfFrame = 0;
	
	for(int i = 0; i < kNumPerfs; i++)
		m_perfs[i][m_perfFrame] = 0.0f;
	
}

float RudePerf::GetStat(eRudePerfStat stat)
{
	return m_perfs[stat][m_perfFrame];
}


void RudePerf::GetStatMinMaxAvg(eRudePerfStat stat, float &min, float &max, float &avg)
{
	avg = 0.0f;
	min = 99999.0f;
	max = 0.0f;
	
	for(int i = 0; i < kNumPerfFrames; i++)
	{
		avg += m_perfs[stat][i];
		
		if(m_perfs[stat][i] > max)
			max = m_perfs[stat][i];
		if(m_perfs[stat][i] < min)
			min = m_perfs[stat][i];
	}
	
	avg /= ((float) kNumPerfFrames);
	
}

void RudePerf::PrintAll()
{
#ifdef RUDE_PERF
	
	float y = 60.0f;
	float yi = 15.0f;
	
	for(int i = 0; i < kNumPerfs; i++)
	{
		//RUDE_REPORT("%20s: %2.2fms\n", kStatNames[i], m_perfs[i]);
		
		float min, max, avg;
		GetStatMinMaxAvg((eRudePerfStat) i, min, max, avg);
		
		RudeText::Print(0.0f, y, 0.5f, 0xFF00FF00, "%s", kStatNames[i]);
		RudeText::Print(200.0f, y, 0.5f, 0xFF00FF00, "%.2f / %.2f", avg, max);
		
		y += yi;
	}
	
#endif
}
