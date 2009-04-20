/*
 *  RBTourTracker.cpp
 *  golf
 *
 *  Created by Robert Rose on 4/19/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "RBTourTracker.h"

#include "RudeRegistry.h"

tRBTourSaveData RBTourTracker::m_data;

void RBTourTracker::Load()
{
	RudeRegistry *reg = RudeRegistry::GetSingleton();
	
	tRBTourSaveData load;
	int loadsize = sizeof(load);
	if(reg->QueryByte("GOLF", "GS_TOUR_STATE", &load, &loadsize) == 0)
	{
		m_data = load;
		
	}
	else
	{
		m_data.m_version = 1;
		for(int i = 0; i < kNumCourses; i++)
			m_data.m_scores[i] = 100;
		
	}
	
}

void RBTourTracker::Save()
{
	RudeRegistry *reg = RudeRegistry::GetSingleton();
	
	m_data.m_version = 1;
	reg->SetByte("GOLF", "GS_TOUR_STATE", &m_data, sizeof(m_data));
	
}


void RBTourTracker::SetScore(int course, int score)
{
	RUDE_ASSERT(course >= 0, "Invalid course");
	
	if(score < m_data.m_scores[course])
	{
		m_data.m_scores[course] = score;
		
		Save();
	}
}
