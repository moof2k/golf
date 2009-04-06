/*
 *  RBCourseData.cpp
 *  golf
 *
 *  Created by Robert Rose on 10/30/08.
 *  Copyright 2008 Bork 3D LLC. All rights reserved.
 *
 */

#include "RBCourseData.h"

RBCourseEntry sCourseData[kNumCourses] = {
{ "Creekside Country Club", 0, kCourseFront9, kCourseShortTee },
{ "Creekside Country Club", 0, kCourseBack9, kCourseShortTee },
{ "Creekside Country Club", 0, kCourseFront9, kCourseLongTee },
{ "Creekside Country Club", 0, kCourseBack9, kCourseLongTee },
{ "Creekside Country Club", 0, kCourseAll18, kCourseShortTee },
{ "Creekside Country Club", 0, kCourseAll18, kCourseLongTee },
{ "Creekside Driving Range", 1, kCourseDrivingRange, kCourseShortTee },
};


RBCourseHole sCourseHoleData[kNumPhysicalCourses][18] = {
{
	{ 1, 5, "par4_A"},
	{ 2, 5, "par5_A" },
	{ 3, 3, "par3_A" },
	{ 4, 4, "par4_E" },
	{ 5, 5, "par5_B" },
	{ 6, 4, "par4_G" },
	{ 7, 3, "par3_B" },
	{ 8, 5, "par5_C" },
	{ 9, 4, "par4_B" },
	{ 10, 4, "par4_H" },
	{ 11, 3, "par3_C" },
	{ 12, 5, "par5_D" },
	{ 13, 4, "par4_C" },
	{ 14, 3, "par3_D" },
	{ 15, 4, "par4_D" },
	{ 16, 3, "par3_E" },
	{ 17, 5, "par5_E" },
	{ 18, 4, "par4_F" }
},

{
	{ 1, 1, "drivingrange" },
	{ 1, 1, "drivingrange" },
	{ 1, 1, "drivingrange" },
	{ 1, 1, "drivingrange" },
	{ 1, 1, "drivingrange" },
	{ 1, 1, "drivingrange" },
	{ 1, 1, "drivingrange" },
	{ 1, 1, "drivingrange" },
	{ 1, 1, "drivingrange" },
	{ 1, 1, "drivingrange" },
	{ 1, 1, "drivingrange" },
	{ 1, 1, "drivingrange" },
	{ 1, 1, "drivingrange" },
	{ 1, 1, "drivingrange" },
	{ 1, 1, "drivingrange" },
	{ 1, 1, "drivingrange" },
	{ 1, 1, "drivingrange" },
	{ 1, 1, "drivingrange" },
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
		case kCourseDrivingRange:
		{
			actualhole = 0;
		}
			break;
	}
	
	return &sCourseHoleData[sCourseData[course].m_physicalCourse][actualhole];
}
