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

class RBTHole {
public:

	int m_holeNum;
	int m_par;
	char m_terrainFile[32];
	
};


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
	
private:
	
	eRBTRoundState m_state;
	RBTGame *m_game;
	
	RudeTextControl m_loadingText;
	
	int m_hole;
	
	int m_numPlayers;
	
};


#endif


