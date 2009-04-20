/*
 *  RBGame.h
 *  golf
 *
 *  Created by Robert Rose on 10/10/08.
 *  Copyright 2008 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef _H_RBGame
#define _H_RBGame


#include "RudeGame.h"
#include "RudeTouchTracker.h"
#include "RBKeyMap.h"

#include "RBTRound.h"
#include "RBUITitle.h"

#include "RBTourTracker.h"

typedef enum {
	kGameNone,
	kGameIntroLogo = 1,
	kGameTitle,
	kGameRBT,
	
} eGameState;



#define kRBTMaxPieces 256
#define kRBTHeight 23
#define kRBTWidth 10

typedef struct {
	eGameState state;
	int course;
	
} tRBGameSaveState;


class RBGame : public RudeGame
{
// Construction
public:
	RBGame();
	~RBGame();


	void Init();
	void Destroy();

	void Render(float delta, float aspect);
	
	void KeyUp(RudeKey k);
	void KeyDown(RudeKey k);
	void StylusDown(RudeScreenVertex &p);
	void StylusMove(RudeScreenVertex &p);
	void StylusUp(RudeScreenVertex &p);
	
	void TouchDown(RudeScreenVertex &n);
	void TouchMove(RudeScreenVertex &n, RudeScreenVertex &p);
	void TouchUp(RudeScreenVertex &n, RudeScreenVertex &p);

	void Pause();

private:
	
	void SaveState();
	int LoadState();
	
	void SetState(eGameState state);

	eGameState	m_state;
	RBGameBase	*m_game;
	int			m_course;

	RBTRound	*m_rbt;
	RBUITitle	*m_uiTitle;

	RBKeyMap	*m_keymap;
	
	RudeTouchTracker m_touchtracker;

};

#endif 
