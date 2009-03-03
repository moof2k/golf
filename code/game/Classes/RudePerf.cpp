/*
 *  RudePerf.cpp
 *  golf
 *
 *  Created by Robert Rose on 3/2/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "RudePerf.h"
#include "RudeDebug.h"
#include "RudeText.h"

int RudePerf::m_perfFrame = 0;
float RudePerf::m_perfs[kNumPerfs][kNumPerfFrames];

const char * kStatNames[kNumPerfs] = {
	"RBTGame::NextFrame",
	"RBTGame::Render",
	"SkinMesh::Render",
};

void RudePerf::NextFrame(float delta)
{
	m_perfFrame += 1;
	
	if(m_perfFrame > kNumPerfFrames)
		m_perfFrame = 0;
	
	for(int i = 0; i < kNumPerfs; i++)
		m_perfs[i][m_perfFrame] = 0.0f;
	
}

float RudePerf::GetStat(eRudePerfStat stat)
{
	return m_perfs[stat][m_perfFrame];
}

float RudePerf::GetStatAverage(eRudePerfStat stat)
{
	float c = 0.0f;
	
	for(int i = 0; i < kNumPerfFrames; i++)
	{
		c += m_perfs[stat][i];
	}
	
	c /= ((float) kNumPerfFrames);
	
	return c;
}

void RudePerf::PrintAll()
{
#ifdef RUDE_PERF
	
	float y = 60.0f;
	float yi = 15.0f;
	
	for(int i = 0; i < kNumPerfs; i++)
	{
		//RUDE_REPORT("%20s: %2.2fms\n", kStatNames[i], m_perfs[i]);
		
		RudeText::Print(0.0f, y, 0.5f, 0xFF00FF00, "%s", kStatNames[i]);
		RudeText::Print(200.0f, y, 0.5f, 0xFF00FF00, "%.2fms", GetStatAverage((eRudePerfStat) i));
		
		y += yi;
	}
	
#endif
}
