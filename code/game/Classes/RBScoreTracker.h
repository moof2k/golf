/*
 *  RBScoreTracker.h
 *  golf
 *
 *  Created by Robert Rose on 10/19/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __H_RBScoreTracker
#define __H_RBScoreTracker

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
	void ClearScores();
	
	int GetNumStrokes(int hole);
	void AddStrokes(int hole, int num);
	
	int GetScore(int hole, bool includeThisHole);
	
private:

	RBScoreEntry m_scores[kNumScores];
};

const int kNumScoreTrackers = 4;

RBScoreTracker * GetScoreTracker(int n);

#endif
