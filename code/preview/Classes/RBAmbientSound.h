/*
 *  RBAmbientSound.h
 *  golf
 *
 *  Created by Robert Rose on 5/3/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __H_RBAmbientSound
#define __H_RBAmbientSound

#include "Rude.h"

class RBAmbientSound
{
public:
	
	RBAmbientSound();
	
	void NextFrame(float delta);
	
	void SetEnabled(bool e);
	
private:
	
	float m_timer;
	bool m_enabled;
	
};

#endif

