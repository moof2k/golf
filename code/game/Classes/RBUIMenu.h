/*
 *  RBUIMenu.h
 *  golf
 *
 *  Created by Robert Rose on 10/30/08.
 *  Copyright 2008 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef __H_RBUIMenu
#define __H_RBUIMenu

#include "RBGameBase.h"
#include "RudeTextControl.h"
#include "RBScoreControl.h"
#include "RBCourseData.h"

typedef enum {
	kMenuQuit,
	kMenuResume,
} eMenuResult;

class RBUIMenu : public RBGameBase {
	
public:
	RBUIMenu();
	~RBUIMenu();
	
	void Reset(int holeNum, eCourseHoles holeSet);
	
	void NextFrame(float delta);
	void Render(float width, float height, int camera);
	
	void TouchDown(RudeTouch *rbt);
	void TouchMove(RudeTouch *rbt);
	void TouchUp(RudeTouch *rbt);

	void Resize();
	
	eMenuResult GetResult() { return m_result; }
	
private:

	RudeControl m_ui;
	
	RBScoreControl *m_scoreControl;
	
	RudeTextControl *m_resumeText;
	RudeTextControl *m_quitText;
	
	float m_bgTimer;
	eMenuResult m_result;
};

#endif
