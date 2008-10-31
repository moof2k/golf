/*
 *  RBUITitle.h
 *  golf
 *
 *  Created by Robert Rose on 10/29/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __H_RBUITitle
#define __H_RBUITitle

#include "RBGameBase.h"
#include "RudeObject.h"
#include "RudeSkybox.h"
#include "RudeButtonControl.h"
#include "RudeTextControl.h"
#include "RBUICourseControl.h"
#include "RBCamera.h"
#include "RBCourseData.h"

typedef enum {
	kTitleNone,
	kTitleSplash,
	kTitleCourseSelect,
	kTitleHighScores,
	kTitleGameOptions,
	kTitleReadyToPlay
} eTitleState;


class RBUITitle : public RBGameBase {
	
public:
	RBUITitle();
	~RBUITitle();
	
	void NextFrame(float delta);
	void Render(float aspect);
	
	void TouchDown(RudeTouch *rbt);
	void TouchMove(RudeTouch *rbt);
	void TouchUp(RudeTouch *rbt);
	
	void Reset();
	
	int GetCourseSelection() { return m_course; }
	
private:
	
	void SetState(eTitleState state);
	
	RudeObject m_terrain;
	RudeSkybox m_skybox;
	RBCamera m_camera;
	
	RudeButtonControl m_logo;
	
	RBUICourseControl m_courseButtons[kNumCourses];
	
	RudeTextControl m_startText;
	RudeTextControl m_continueText;
	RudeTextControl m_copyrightText;
	RudeTextControl m_backText;
	
	RudeTextControl m_courseNameText;
	RudeTextControl m_courseHolesText;
	RudeTextControl m_courseTeeText;
	
	RudeTextControl m_goText;
	
	eTitleState m_state;
	
	float m_cameraTimer;
	float m_readyTimer;
	int m_course;
};


#endif
