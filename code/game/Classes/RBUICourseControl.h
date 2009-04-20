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
	
	RBUICourseControl()
	: m_name(0)
	, m_subname(0)
	, m_desc(0)
	, m_holes(kCourseFront9)
	, m_tee(kCourseShortTee)
	{
	}
	
	void Render();
	
	const char * GetNameStr() { return m_name; }
	const char * GetSubnameStr() { return m_subname; }
	const char * GetDescStr() { return m_desc; }
	const char * GetHoleStr();
	const char * GetTeeStr();
	
	const char *m_name;
	const char *m_subname;
	const char *m_desc;
	eCourseHoles m_holes;
	eCourseTee m_tee;
	
};

#endif
