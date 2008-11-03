/*
 *  RBScoreTracker.cpp
 *  golf
 *
 *  Created by Robert Rose on 10/19/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "RBScoreTracker.h"
#include "Rude.h"

RBScoreTracker::RBScoreTracker()
{
	
}


void RBScoreTracker::SetPar(int hole, int par)
{
	RUDE_ASSERT(hole < kNumScores, "Invalid hole");
	
	m_scores[hole].m_par = par;
}

int RBScoreTracker::GetPar(int hole)
{
	RUDE_ASSERT(hole < kNumScores, "Invalid hole");
	
	return m_scores[hole].m_par;
}

void RBScoreTracker::ClearScores()
{
	for(int i = 0; i < kNumScores; i++)
	{
		m_scores[i].Clear();
	}
}

int RBScoreTracker::GetNumStrokes(int hole)
{
	RUDE_ASSERT(hole < kNumScores, "Invalid hole");
	
	return m_scores[hole].m_strokes;
}

void RBScoreTracker::AddStrokes(int hole, int num)
{
	RUDE_ASSERT(hole < kNumScores, "Invalid hole");
	
	m_scores[hole].m_strokes += num;
}

int RBScoreTracker::GetScore(eCourseHoles holeSet, int hole, bool includeThisHole)
{
	int lasthole = hole - 1;
	if(includeThisHole)
		lasthole = hole;
	
	if(lasthole < 0)
		return 0;
	
	int score = 0;
	
	int starthole = 0;
	
	if(holeSet == kCourseBack9)
		starthole = 9;
	
	for(int i = starthole; i <= lasthole; i++)
	{
		int par = m_scores[i].m_par;
		int strokes = m_scores[i].m_strokes;
		
		score += (strokes - par);
	}
	
	return score;
}


RBScoreTracker gScoreTrackers[kNumScoreTrackers];

RBScoreTracker * GetScoreTracker(int n)
{
	RUDE_ASSERT(n >= 0, "Invalid score tracker number");
	RUDE_ASSERT(n < kNumScoreTrackers, "Invalid score tracker number");
	
	return &gScoreTrackers[n];
}