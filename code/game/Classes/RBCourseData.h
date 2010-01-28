/*
 *  RBCourseData.h
 *  golf
 *
 *  Created by Robert Rose on 10/30/08.
 *  Copyright 2008 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef __H_RBCourseData
#define __H_RBCourseData

#include "Rude.h"


const int kNumCourses = 7;
const int kNumPhysicalCourses = 2;

typedef enum {
	kCourseFront9,
	kCourseBack9,
	kCourseAll18,
	kCourseDrivingRange
} eCourseHoles;

typedef enum {
	kCourseShortTee,
	kCourseLongTee
} eCourseTee;

typedef enum {
	kCourseNoWind,
	kCourseLowWind,
	kCourseHighWind
} eCourseWind;

class RBCourseEntry {
public:
	const char *m_name;
	const char *m_subname;
	const char *m_desc;
	const char *m_image;
	float m_imageOffset;
	int m_completionScore;
	int m_physicalCourse;
	eCourseHoles m_holes;
	eCourseTee m_tee;
	eCourseWind m_wind;
};

class RBCourseHole {
public:
	
	int m_holeNum;
	int m_par;
	char m_terrainFile[32];
	
};

extern RBCourseEntry sCourseData[kNumCourses];

RBCourseEntry * GetCourseData(int c);
RBCourseHole * GetCourseHole(int course, eCourseHoles holeset, int hole);


#endif
