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

#include "RudeControl.h"
#include "RudeObject.h"
#include "RBBallGuide.h"

class RBTerrainSliceControl : public RudeControl {
public:
	RBTerrainSliceControl();
	
	void NextFrame(float delta);
	void Render();
	
	void SetBallGuide(RBBallGuide *guide) { m_guide = guide; }
	
private:
	
	RBBallGuide *m_guide;

};

#endif
