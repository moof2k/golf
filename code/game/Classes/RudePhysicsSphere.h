/*
 *  RudePhysicsSphere.h
 *  golf
 *
 *  Created by Robert Rose on 9/8/08.
 *  Copyright 2008 Bork 3D LLC. All rights reserved.
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

