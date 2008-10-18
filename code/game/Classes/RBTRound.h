/*
 *  RBTRound.h
 *  golf
 *
 *  Created by Robert Rose on 10/17/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __H_RBTRound
#define __H_RBTRound


#include "Rude.h"
#include "RudeTouchTracker.h"
#include "RBTGame.h"
#include "RudeTextControl.h"

typedef enum {
	kStateInit,
	kStateNextRound,
	kStateInRound,
	kStateDone,
} eRBTRoundState;

class RBTRound : public RudeGame {
	
public:
	RBTRound();
	~RBTRound();
	
	void NextFrame(float delta);
	void Render(float aspect);
	
	void TouchDown(RudeTouch *rbt);
	void TouchMove(RudeTouch *rbt);
	void TouchUp(RudeTouch *rbt);
	
	
private:
	
	eRBTRoundState m_state;
	RBTGame *m_game;
	
	RudeTextControl m_loadingText;
	
};


#endif


