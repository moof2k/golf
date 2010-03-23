/*
 *  RBUITutorial.h
 *  golf
 *
 *  Created by Robert Rose on 4/28/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __H_RBUITutorial
#define __H_RBUITutorial

#include "RBGameBase.h"

class RBUITutorial : public RBGameBase
{
public:
	RBUITutorial();
	
	void NextFrame(float delta);
	void Render(float width, float height);
	
	void TouchDown(RudeTouch *rbt);
	void TouchMove(RudeTouch *rbt);
	void TouchUp(RudeTouch *rbt);
	
	void Reset();
	
	void SetCourse(int course) { m_course = course; }

	
private:

	void NextImage();
	
	float m_timer;
	
	int m_course;
	int m_curImage;
	
	int m_texture;
	
	int m_imageDelay;
	
	bool m_runcheck;
	
};


#endif
