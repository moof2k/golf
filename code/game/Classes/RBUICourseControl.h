/*
 *  RBUICourseControl.h
 *  golf
 *
 *  Created by Robert Rose on 10/29/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __H_RBUICourseControl
#define __H_RBUICourseControl

#include "Rude.h"
#include "RudeButtonControl.h"

typedef enum {
	kCourseFront9,
	kCourseBack9,
	kCourseAll18
} eCourseHoles;

typedef enum {
	kCourseShortTee,
	kCourseLongTee
} eCourseTee;

const int kMaxCourseNameLen = 64;

class RBUICourseControl : public RudeButtonControl {
	
public:
	
	void Render();
	
	
	const char *m_name;
	eCourseHoles m_holes;
	eCourseTee m_tee;
	
};

#endif
