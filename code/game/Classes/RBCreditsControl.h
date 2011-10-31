/*
 *  RBCreditsControl.h
 *  golf
 *
 *  Created by Robert Rose on 4/25/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __H_RBCreditsControl
#define __H_RBCreditsControl


#include "RudeControl.h"

class RBCreditsControl : public RudeControl
{
	
public:
	RBCreditsControl(RudeControl *parent);
	
	void Render();
	
private:
	
};

#endif
