/*
 *  RudePhysicsMesh.h
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
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


class RudePhysicsMesh : public RudePhysicsObject {
	
public:
	RudePhysicsMesh(RudeObject *owner);
	virtual ~RudePhysicsMesh();
	
	virtual void Load(RudeMesh *mesh, float mass);
	
	int GetSubPartMapping(int part) { return m_subPartMappings[part]; }
	
protected:
	
	std::map<int, int> m_subPartMappings;
	
	btTriangleIndexVertexArray * m_data;
	btBvhTriangleMeshShape *m_shape;
	
};


#endif
