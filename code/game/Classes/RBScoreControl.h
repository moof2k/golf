/*
 *  RBScoreControl.h
 *  golf
 *
 *  Created by Robert Rose on 10/19/08.
 *  Copyright 2008 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef __H_RBScoreControl
#define __H_RBScoreControl

#include "Rude.h"
#include "RudeControl.h"
#include "RBCourseData.h"

typedef enum {
	kScoreColorNormal,
	kScoreColorHole,
	kScoreColorActiveHole
} eRBScoreColor;


class RBScoreControl : public RudeControl {
	
public:
	RBScoreControl(RudeControl *parent);
	
	void Render();
	
	void SetActiveHole(int hole, eCourseHoles holeSet) { m_activeHole = hole; m_holeSet = holeSet; }
	
private:
	
	void RenderScoreBoard(int x, int y, int start);
	
	void Printf(float x, float y, int align, eRBScoreColor color, const char *fmt, int value);
	
	int m_activeHole;
	eCourseHoles m_holeSet;
};

#endif
