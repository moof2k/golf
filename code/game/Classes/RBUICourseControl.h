/*
 *  RBUICourseControl.h
 *  golf
 *
 *  Created by Robert Rose on 10/29/08.
 *  Copyright 2008 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef __H_RBUICourseControl
#define __H_RBUICourseControl

#include "Rude.h"
#include "RudeButtonControl.h"
#include "RBCourseData.h"

const int kMaxCourseNameLen = 64;

class RBUICourseControl : public RudeButtonControl {
	
public:
	
	void Render();
	
	const char * GetNameStr() { return m_name; }
	const char * GetHoleStr();
	const char * GetTeeStr();
	
	const char *m_name;
	eCourseHoles m_holes;
	eCourseTee m_tee;
	
};

#endif
