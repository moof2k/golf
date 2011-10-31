/*
 *  RBSwingHelper.h
 *  golf
 *
 *  Created by Robert Rose on 4/26/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __H_RBSwingHelper
#define __H_RBSwingHelper

#include "Rude.h"

#include "RudeControl.h"

class RBSwingHelper : public RudeControl
{
public:
	RBSwingHelper(RudeControl *parent);
	
	void NextFrame(float delta);
	void Render();
	
	void SetEnabled(bool e);
	void SetShortTimer(bool st) { m_shortTimer = st; }
	
private:
	
	float m_delaytimer;
	float m_postimer;
	bool m_posdown;
	bool m_cycling;
	bool m_enabled;
	bool m_shortTimer;
};




#endif
