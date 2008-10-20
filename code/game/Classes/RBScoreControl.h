/*
 *  RBScoreControl.h
 *  golf
 *
 *  Created by Robert Rose on 10/19/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __H_RBScoreControl
#define __H_RBScoreControl

#include "Rude.h"
#include "RudeControl.h"

typedef enum {
	kScoreColorNormal
} eRBScoreColor;


class RBScoreControl : public RudeControl {
	
public:
	RBScoreControl();
	
	void Render();
	
	
	
private:
	
	void RenderScoreBoard(int x, int y, int start);
	
	void Printf(float x, float y, int align, eRBScoreColor color, const char *fmt, int value);
	
};

#endif
