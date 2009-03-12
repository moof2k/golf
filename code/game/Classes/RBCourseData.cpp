/*
 *  RBCourseData.cpp
 *  golf
 *
 *  Created by Robert Rose on 10/30/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "RBCourseData.h"

RBCourseEntry sCourseData[kNumCourses] = {
{ "Creekside Country Club", 0, kCourseFront9, kCourseShortTee },
{ "Creekside Country Club", 0, kCourseBack9, kCourseShortTee },
{ "Creekside Country Club", 0, kCourseFront9, kCourseLongTee },
{ "Creekside Country Club", 0, kCourseBack9, kCourseLongTee },
{ "Creekside Country Club", 0, kCourseAll18, kCourseShortTee },
{ "Creekside Country Club", 0, kCourseAll18, kCourseLongTee }
};


RBCourseHole sCourseHoleData[kNumPhysicalCourses][18] = {
{
	{ 1, 3, "parfive"},
	{ 2, 3, "par3_B" },
	{ 3, 3, "par3_C" },
	{ 4, 3, "par3_D" },
	{ 5, 3, "par3_E" },
	{ 6, 5, "parfive" },
	{ 7, 5, "parfive" },
	{ 8, 5, "parfive" },
	{ 9, 5, "parfive" },
	{ 10, 5, "parfive" },
	{ 11, 5, "parfive" },
	{ 12, 5, "parfive" },
	{ 13, 5, "parfive" },
	{ 14, 5, "parfive" },
	{ 15, 5, "parfive" },
	{ 16, 5, "parfive" },
	{ 17, 5, "parfive" },
	{ 18, 5, "parfive" }
}

};

RBCourseEntry * GetCourseData(int course)
{
	RUDE_ASSERT(course >= 0 && course < kNumCourses, "Invalid course number");
	
	return &sCourseData[course];
}

RBCourseHole * GetCourseHole(int course, eCourseHoles holeset, int hole)
{
	RUDE_ASSERT(course >= 0 && course < kNumCourses, "Invalid course number");
	RUDE_ASSERT(hole >= 0, "Invalid hole number");
	
	int actualhole = 0;
	
	switch(holeset)
	{
		case kCourseFront9:
		{
			if(hole >= 9)
				return 0;
			
			actualhole = hole;
		}
			break;
		case kCourseBack9:
		{
			if(hole >= 18)
				return 0;
			
			actualhole = hole;
		}
			break;
		case kCourseAll18:
		{
			if(hole >= 18)
				return 0;
			
			actualhole = hole;
		}
			break;
	}
	
	return &sCourseHoleData[sCourseData[course].m_physicalCourse][actualhole];
}
