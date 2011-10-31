/*
 *  RBTerrainSliceControl.h
 *  golf
 *
 *  Created by Robert Rose on 10/30/08.
 *  Copyright 2008 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef __H_RBTerrainSliceControl
#define __H_RBTerrainSliceControl

#include "RudeButtonControl.h"
#include "RudeControl.h"
#include "RudeObject.h"
#include "RBBallGuide.h"

class RBTerrainSliceControl : public RudeControl {
public:
	RBTerrainSliceControl(RudeControl *parent);
	
	void NextFrame(float delta);
	void Render();
	
	void SetBallGuide(RBBallGuide *guide) { m_guide = guide; }
	void SetCoursePositions(const btVector3 &ball, const btVector3 &hole, const btVector3 &guide);
	
private:
	
	RudeButtonControl m_guideIndicatorButton;
	RudeButtonControl m_holeIndicatorButton;
	
	RBBallGuide *m_guide;
	
	float m_holeDistance;
	float m_guideDistance;

};

#endif
