/*
 *  RBTerrain.h
 *  golf
 *
 *  Created by Robert Rose on 9/8/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __H_RBTerrain
#define __H_RBTerrain

#include "RudeObject.h"

#include <btBulletDynamicsCommon.h>

#include <vector>


class RBTerrain : public RudeObject {

public:
	RBTerrain();
	virtual ~RBTerrain() {}
	
	void Load(const char *name);
	
	void Contact(RudePhysicsObject *other, int terrainId, int otherId);
	
	btVector3 GetTeeBox() { return m_teeBox; }
	btVector3 GetGuidePoint(btVector3 ball);
	btVector3 GetHole() { return m_hole; }
	
	bool GetBallInHole() { return m_ballInHole; }
	void SetBallInHole(bool b) { m_ballInHole = b; }
	
	virtual void Render();
	
private:
	
	void LoadNodes();
	void RenderHole();
	
	btVector3 m_teeBox;
	btVector3 m_hole;
	
	std::vector<btVector3> m_shortTees;
	std::vector<btVector3> m_longTees;
	std::vector<btVector3> m_holes;
	std::vector<btVector3> m_guidePoints;
	std::vector<btVector3> m_cameraPlacements;
	
	int m_holeTexture;
	
	bool m_ballInHole;
};

#endif

