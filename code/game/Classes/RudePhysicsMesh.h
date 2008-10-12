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


class RudePhysicsMesh : public RudePhysicsObject {
	
public:
	RudePhysicsMesh(RudeObject *owner);
	virtual ~RudePhysicsMesh();
	
	virtual void Load(RudeMesh *mesh, float mass);
	
	
	
protected:
	
	btTriangleIndexVertexArray * m_data;
	btBvhTriangleMeshShape *m_shape;
	
};


#endif
