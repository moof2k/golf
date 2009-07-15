/*
 *  RudePhysicsSphere.cpp
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#include "RudePhysicsSphere.h"
#include "RudePhysics.h"

#include <btBulletDynamicsCommon.h>

RudePhysicsSphere::RudePhysicsSphere(RudeObject *owner)
: RudePhysicsObject(owner)
{
}

RudePhysicsSphere::~RudePhysicsSphere()
{
	if(m_shape)
		delete m_shape;
}

void RudePhysicsSphere::Load(float radius, float mass)
{
	m_shape = new btSphereShape(radius);
	
	m_motionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,100,0)));
	
	btVector3 inertia(0,0,0);
	m_shape->calculateLocalInertia(mass,inertia);
	
	btRigidBody::btRigidBodyConstructionInfo rigidbodyci(mass,m_motionState,m_shape,inertia);
	m_rigidBody = new btRigidBody(rigidbodyci);

	
	RudePhysics::GetInstance()->AddObject(this);
	
	//m_rigidBody->applyForce(btVector3(10,0,0), btVector3(0,0,0));
}

