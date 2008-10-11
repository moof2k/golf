/*
 *  RudeButtonControl.h
 *  golf
 *
 *  Created by Robert Rose on 9/16/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __H_RudeButtonControl
#define __H_RudeButtonControl

#include "RudeControl.h"

class RudeButtonControl : public RudeControl
{
public:
	
	RudeButtonControl();
	
	void SetTextures(const char *stateoff, const char *stateon);
	
	virtual bool TouchDown(RudeTouch *t);
	virtual bool TouchMove(RudeTouch *t);
	virtual bool TouchUp(RudeTouch *t);
	
	virtual void Render();
	
protected:
	
	bool m_state;
	int m_stateofftex;
	int m_stateontex;
	
	int m_texsize;
	
};

#endif
