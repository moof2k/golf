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
#include "RBGameBase.h"


class RBGame : public RudeGame
{
// Construction
public:
	RBGame();
	~RBGame();


	void Init();
	void Destroy();

	void Render(float delta, float width, float height);
	
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

	RBGameBase *m_game;

	RBKeyMap	*m_keymap;
	
	RudeTouchTracker m_touchtracker;

};

#endif 
