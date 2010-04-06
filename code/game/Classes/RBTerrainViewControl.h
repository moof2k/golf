/*
 *  RBTerrainViewControl.h
 *  golf
 *
 *  Created by Robert Rose on 10/30/08.
 *  Copyright 2008 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef __H_RBTerrainViewControl
#define __H_RBTerrainViewControl

#include "RudeControl.h"
#include "RudeObject.h"
#include "RBTerrain.h"

class RBTerrainViewControl : public RudeControl {
public:
	RBTerrainViewControl();
	
	void NextFrame(float delta);
	void Render();
	
	void SetTerrain(RBTerrain *terrain)
	{
		RUDE_ASSERT(terrain, "Invalid terrain");

		m_terrain = terrain;
	}

	void SetPositions(const btVector3 &ball, const btVector3 &hole);
	
private:
	
	RBTerrain *m_terrain;

	float m_originx;
	float m_originy;
	float m_angle;
	float m_scale;
	
};

#endif
