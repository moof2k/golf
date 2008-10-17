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
#include "RBTerrainMaterial.h"

#include <btBulletDynamicsCommon.h>

#include <vector>
#include <map>

class RBTerrainMaterialInfo {

public:
	float m_friction;
	float m_restitution;
	
	float m_linearDamping;
	float m_angularDamping;
	
	float m_minVelocity;
	
};


class RBTerrain : public RudeObject {

public:
	RBTerrain();
	virtual ~RBTerrain() {}
	
	void Load(const char *name);
	
	void Contact(RudePhysicsObject *other, int terrainId, int otherId, float *friction, float *restitution);
	
	bool IsInBounds(const btVector3 &position);
	
	btVector3 GetTeeBox() { return m_teeBox; }
	btVector3 GetGuidePoint(btVector3 ball);
	btVector3 GetHole() { return m_hole; }
	btVector3 GetCameraPlacement(btVector3 ball);
	
	bool GetBallInHole() { return m_ballInHole; }
	void SetBallInHole(bool b) { m_ballInHole = b; }
	
	virtual void Render();
	
private:
	
	void LoadMaterials();
	void LoadNodes();
	void RenderHole();
	
	btVector3 m_teeBox;
	btVector3 m_hole;
	
	std::map<int, eRBTerrainMaterial> m_terrainParts;
	
	std::vector<btVector3> m_shortTees;
	std::vector<btVector3> m_longTees;
	std::vector<btVector3> m_holes;
	std::vector<btVector3> m_guidePoints;
	std::vector<btVector3> m_cameraPlacements;
	
	int m_holeTexture;
	
	bool m_ballInHole;
};

#endif

