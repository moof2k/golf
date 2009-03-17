/*
 *  RBTRound.h
 *  golf
 *
 *  Created by Robert Rose on 10/17/08.
 *  Copyright 2008 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef __H_RBTRound
#define __H_RBTRound


#include "Rude.h"
#include "RudeTouchTracker.h"
#include "RBTGame.h"
#include "RudeTextControl.h"
#include "RBCourseData.h"

typedef enum {
	kStateInit,
	kStateNextRound,
	kStateInRound,
	kStateDone,
} eRBTRoundState;

typedef struct {
	eRBTRoundState state;
	int hole;
	eCourseHoles holeset;
	eCourseTee tee;
	eCourseWind wind;
} tRBTRoundSaveState;

const int kMaxPlayers = 4;


class RBTRound : public RBGameBase {
	
public:
	RBTRound();
	~RBTRound();
	
	void NextFrame(float delta);
	void Render(float aspect);
	
	void TouchDown(RudeTouch *rbt);
	void TouchMove(RudeTouch *rbt);
	void TouchUp(RudeTouch *rbt);
	
	void Reset();
	
	void SetCourse(int course);
	
	void SaveState();
	int LoadState();
	
private:
	
	void RestoreState();
	void SetState(eRBTRoundState state);
	
	eRBTRoundState m_state;
	RBTGame *m_game;
	
	RudeTextControl m_loadingText;
	
	eCourseHoles m_holeSet;
	eCourseTee m_tee;
	eCourseWind m_wind;
	int m_hole;
	
	int m_numPlayers;
	
};


#endif


