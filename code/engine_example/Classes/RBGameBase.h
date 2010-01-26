/*
 *  RBGameBase.h
 *  golf
 *
 *  Created by Robert Rose on 10/29/08.
 *  Copyright 2008 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef __H_RBGameBase
#define __H_RBGameBase

#include "Rude.h"
#include "RudeDebug.h"
#include "RudeTouchTracker.h"

class RBGameBase {
public:
	
	RBGameBase()
	: m_done(false)
	{
	}
	
	virtual ~RBGameBase()
	{
	}
	
	virtual void NextFrame(float delta) = 0;
	virtual void Render(float width, float height) = 0;
	
	virtual void TouchDown(RudeTouch *rbt) = 0;
	virtual void TouchMove(RudeTouch *rbt) = 0;
	virtual void TouchUp(RudeTouch *rbt) = 0;
	
	
	virtual void Reset() { m_done = false; }
	
	bool Done() { return m_done; }
	
protected:
	
	bool m_done;
};

#endif
