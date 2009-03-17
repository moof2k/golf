/*
 *  RBScoreTracker.h
 *  golf
 *
 *  Created by Robert Rose on 10/19/08.
 *  Copyright 2008 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef __H_RBScoreTracker
#define __H_RBScoreTracker

#include "RBCourseData.h"

const int kNumScores = 18;

class RBScoreEntry {
public:
	int m_par;
	int m_strokes;
	
	RBScoreEntry()
	: m_par(0)
	, m_strokes(0)
	{
	}
	
	void Clear()
	{
		m_strokes = 0;
	}
	
};

class RBScoreTracker {
public:
	
	RBScoreTracker();
	
	void SetPar(int hole, int par);
	int GetPar(int hole);
	
	void ClearScores();
	
	int GetNumStrokes(int hole);
	void AddStrokes(int hole, int num);
	
	int GetScore(eCourseHoles holeSet, int hole, bool includeThisHole);
	
	void SaveState(int n);
	int LoadState(int n);
	
private:

	RBScoreEntry m_scores[kNumScores];
};

const int kNumScoreTrackers = 4;

RBScoreTracker * GetScoreTracker(int n);

#endif
