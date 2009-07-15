/*
 *  RudePhysicsSphere.h
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#pragma once

#include "RudePhysicsObject.h"

class btCollisionShape;

class RudePhysicsSphere : public RudePhysicsObject {

public:
	RudePhysicsSphere(RudeObject *owner);
	virtual ~RudePhysicsSphere();
	
	void Load(float radius, float mass);

private:

	btCollisionShape *m_shape;
};

