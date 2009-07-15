/*
 *  RBTerrain.h
 *  golf
 *
 *  Created by Robert Rose on 9/8/08.
 *  Copyright 2008 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef __H_RBTerrain
#define __H_RBTerrain

#include "RudeObject.h"
#include "RudeMesh.h"
#include "RBTerrainMaterial.h"
#include "RBCourseData.h"
#include "RBDecorators.h"

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
	
	int m_penalty_power_min;
	int m_penalty_power_max;
	float m_penalty_angle;
	
};

const int kMaxGreenVerts = 1024;

class RBTerrain : public RudeObject {

public:
	RBTerrain();
	virtual ~RBTerrain() {}
	
	void Load(const char *name);
	
	void Contact(const btVector3 &contactNormal, RudePhysicsObject *other, int terrainId, int otherId, float *friction, float *restitution);
	
	bool IsOutOfBounds(const btVector3 &position);
	bool IsOutOfBoundsAndGone(const btVector3 &position);
	
	void UpdateGuidePoint(const btVector3 &ball, float clubDistance);
	
	RBTerrainMaterialInfo & GetMaterialInfo(eRBTerrainMaterial type);
	
	btVector3 GetTeeBox();
	btVector3 GetGuidePoint() { return m_guidePoint; }
	btVector3 GetHole() { return m_hole; }
	void SetHole(const btVector3 hole) { m_hole = hole; }
	btVector3 GetCameraPlacement(btVector3 ball);
	
	bool GetBallInHole() { return m_ballInHole; }
	void SetBallInHole(bool b) { m_ballInHole = b; }
	
	void SetTee(eCourseTee tee) { m_tee = tee; }
	
	bool GetPutting() { return m_isPutting; }
	void SetPutting(bool isPutting);
	void SetEnablePuttingGreen(bool enablePuttingGreen);
	
	void FinalizeGuidePoints();
	
	bool CastToTerrain(const btVector3 &start, const btVector3 &end, btVector3 &result);
	
	void DropDecorator(const btVector3 &pos, float size) { m_decorator.Drop(pos, size); }
	void DumpDecorator() { m_decorator.Print(); }
	
	virtual void Render();
	
private:
	
	void UpdatePutting();
	
	void LoadMaterials();
	void LoadNodes();
	void RenderHole();
	void RenderGuides();
	
	RBDecoratorCollection m_decorator;
	
	unsigned int m_greenVerts[kMaxGreenVerts];
	int m_greenNode;
	
	btVector3 m_hole;
	btVector3 m_ball;
	
	std::map<int, eRBTerrainMaterial> m_terrainParts;
	
	std::vector<btVector3> m_teeBoxes;
	std::vector<btVector3> m_holes;
	std::vector<btVector3> m_guidePoints;
	std::vector<btVector3> m_cameraPlacements;
	
	btVector3 m_guidePoint;
	
	int m_holeTexture;
	eCourseTee m_tee;
	
	bool m_ballInHole;
	bool m_isPutting;
	bool m_enablePuttingGreen;
};

#endif

