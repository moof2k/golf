/*
 *  RBTourTracker.h
 *  golf
 *
 *  Created by Robert Rose on 4/19/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __H_RBTourTracker
#define __H_RBTourTracker


#include "Rude.h"
#include "RBCourseData.h"


typedef struct {
	int m_version;
	
	int m_scores[kNumCourses];
	
} tRBTourSaveData;


class RBTourTracker
{
public:
	
	static void Load();
	static void Save();
	
	static void SetScore(int course, int score);
	static int GetScore(int course) { return m_data.m_scores[course]; }
	
private:
	
	static tRBTourSaveData m_data;
	
};


#endif
