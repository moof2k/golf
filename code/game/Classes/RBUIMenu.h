/*
 *  RBUIMenu.h
 *  golf
 *
 *  Created by Robert Rose on 10/30/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __H_RBUIMenu
#define __H_RBUIMenu

#include "RBGameBase.h"
#include "RudeTextControl.h"

typedef enum {
	kMenuQuit,
	kMenuResume,
} eMenuResult;

class RBUIMenu : public RBGameBase {
	
public:
	RBUIMenu();
	~RBUIMenu();
	
	void Reset();
	
	void NextFrame(float delta);
	void Render(float aspect);
	
	void TouchDown(RudeTouch *rbt);
	void TouchMove(RudeTouch *rbt);
	void TouchUp(RudeTouch *rbt);
	
	eMenuResult GetResult() { return m_result; }
	
private:
	
	
	RudeTextControl m_resumeText;
	RudeTextControl m_quitText;
	
	float m_bgTimer;
	eMenuResult m_result;
};

#endif
