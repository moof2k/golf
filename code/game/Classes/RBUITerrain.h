/*
 *  RBUITerrain.h
 *  golf
 *
 *  Created by Robert Rose on 4/30/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __H_RBUITerrain
#define __H_RBUITerrain

#include "Rude.h"
#include "RBGameBase.h"
#include "RBTerrain.h"
#include "RBTerrainViewControl.h"

typedef enum {
	kTerrainModeNone,
	kTerrainModeTranslate,
	kTerrainModeScale,
} eTerrainMode;


class RBUITerrain : public RBGameBase {
public:
	
	RBUITerrain();
	~RBUITerrain();
	
	void NextFrame(float delta);
	void Render(float width, float height);

	void StartDisplay();

	void SetPositions(const btVector3 &ball, const btVector3 &hole, const btVector3 &guide)
	{
		m_tvc.SetPositions(ball, hole, guide);
	}
	
	void TouchDown(RudeTouch *rbt);
	void TouchMove(RudeTouch *rbt);
	void TouchUp(RudeTouch *rbt);


	void SetTerrain(RBTerrain *terrain)
	{
		m_tvc.SetTerrain(terrain);
	}

private:
	
	void CalcTouchDist();

	RBTerrainViewControl m_tvc;

	RudeTouch *m_touch[2];
	RudeScreenVertex m_pos[2];
	
	eTerrainMode m_mode;

	float m_startingTouchDist;
	float m_touchDist;

};

#endif
