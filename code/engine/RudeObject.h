/*
 *  RudeObject.h
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef __H_RudeObject
#define __H_RudeObject

#include "Rude.h"

#include <btBulletDynamicsCommon.h>

class RudeMesh;
class RudeSkinnedMesh;
class RudePhysicsObject;

class RudeObject {
	
public:
	
	RudeObject();
	~RudeObject();
	
	virtual void LoadMesh(const char *name);
	
	virtual void LoadSkinnedMesh(const char *name);
	
	RudeMesh * GetMesh() { return m_mesh; }
	
	
	void LoadPhysicsMesh(float mass);
	void LoadPhysicsSphere(float radius, float mass);
	
	virtual void NextFrame(float delta);
	virtual void Render();
	
	RudePhysicsObject * GetPhysicsObject() { return m_physics; }
	
	virtual void SetPosition(const btVector3 &p)
	{
		m_position = p;
	}
	
protected:
	
	btVector3 m_position;
	
private:
	
	RudeMesh *m_mesh;
	RudePhysicsObject *m_physics;
};

#endif


