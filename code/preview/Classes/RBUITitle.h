/*
 *  RBUITitle.h
 *  golf
 *
 *  Created by Robert Rose on 10/29/08.
 *  Copyright 2008 Bork 3D LLC. All rights reserved.
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
#include "RBCreditsControl.h"
#include "RBCourseData.h"
#include "RBDecorators.h"
#include "RBScoreControl.h"

typedef enum {
	kTitleNone,
	kTitleScoreSummary,
	kTitleSplash,
	kTitleSettings,
	kTitleCourseSelect,
	kTitleHighScores,
	kTitleGameOptions,
	kTitlePracticeOptions,
	kTitleReadyToPlay,
	kTitleReadyToPractice,
} eTitleState;

const int kNumCoursesPerScreen = 6;

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
	
	void SetCourseSelection(int course) { m_course = course; }
	int GetCourseSelection() { return m_course; }
	
	void SetCourseScore(int score);
	
	void SetState(eTitleState state);
	
private:
	
	void RefreshScores();
	
	RudeObject m_terrain;
	RBDecoratorCollection m_decorator;
	RudeSkybox m_skybox;
	RudeObject m_flag;
	RBCamera m_camera;
	
	RudeButtonControl m_logo;
	RudeButtonControl m_courseMedallion;
	RudeButtonControl m_courseSplash;
	
	RudeButtonControl m_rangeMedallion;
	
	RBUICourseControl m_courseButtons[kNumCourses];
	
	RBScoreControl m_scoreControl;
	
	RBCreditsControl m_credits;
	
	RudeTextControl m_startText;
	RudeTextControl m_practiceText;
	RudeTextControl m_settingsText;
	RudeTextControl m_copyrightText;
	RudeTextControl m_backText;
	
	RudeTextControl m_tournamentText;
	
	RudeTextControl m_courseNameText;
	RudeTextControl m_courseSubnameText;
	RudeTextControl m_courseDescText;
	RudeTextControl m_courseTeeText;
	
	RudeTextControl m_scoreText;
	RudeTextControl m_scoreDesc;
	
	RudeTextControl m_settingsTitleText;
	RudeTextControl m_musicToggle;
	
	RudeTextControl m_goText;
	
	eTitleState m_state;
	
	float m_cameraTimer;
	float m_readyTimer;
	float m_flagOffset;
	int m_course;
	
	bool m_startedMusic;
};


#endif
