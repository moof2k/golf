/*
 *  RudePhysicsMesh.h
 *  golf
 *
 *  Created by Robert Rose on 9/8/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __H_RudePhysicsMesh
#define __H_RudePhysicsMesh

#include "Rude.h"
#include "RudeObject.h"
#include "RudePhysicsObject.h"

#include <map>
#include <vector>

#include <btBulletDynamicsCommon.h>

class RudeMesh;


class cRudePhysicsMeshMaterial {

public:
	cRudePhysicsMeshMaterial()
	: m_valid(-1)
	, m_friction(1.0f)
	, m_restitution(0.0f)
	{}
	
	cRudePhysicsMeshMaterial(float friction, float restitution)
	: m_valid(1)
	, m_friction(friction)
	, m_restitution(restitution)
	{}
	
	int m_valid;
	float m_friction;
	float m_restitution;
};

class RudePhysicsMesh : public RudePhysicsObject {
	
public:
	RudePhysicsMesh(RudeObject *owner);
	virtual ~RudePhysicsMesh();
	
	virtual void Load(RudeMesh *mesh, float mass);
	
	virtual void GetMaterialProperties(int partId, float *friction, float *restitution);
	
	void AddMaterial(int materialCode, const cRudePhysicsMeshMaterial &material)
	{
		m_materials[materialCode] = material;
	}
	
	
protected:
	
	btTriangleIndexVertexArray * m_data;
	btBvhTriangleMeshShape *m_shape;
	
	std::vector<int> m_materialCodes;
	std::map<int, cRudePhysicsMeshMaterial> m_materials;
};


#endif
