/*
 *  RBKeyTracker.h
 *  golf
 *
 *  Created by Robert Rose on 10/10/08.
 *  Copyright 2008 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef _H_RPGKeyTracker
#define _H_RPGKeyTracker

#include "RBKeyMap.h"

#define kKTMaxKeys 5

class RBKeyTracker
{
public:
	RBKeyTracker(void);
	~RBKeyTracker(void);

	void init(void);

	void keyDown(RBKey k);
	void keyUp(RBKey k);

	bool isDown(RBKey k);

	void getKeyStr(TCHAR *str);

	bool timer(float delta);

private:

	float _keytimer;
	float _timermax;

	RBKey	_keysdown[kKTMaxKeys];

};

#endif
