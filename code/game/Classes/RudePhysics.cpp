/*
 *  RudePhysics.cpp
 *  golf
 *
 *  Created by Robert Rose on 9/8/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "Rude.h"
#include "RudePhysics.h"
#include "RudePhysicsObject.h"
#include "RudeDebug.h"

#include <btBulletDynamicsCommon.h>

inline btScalar	calculateCombinedFriction(float friction0,float friction1)
{
	btScalar friction = friction0 * friction1;
	
	/*const btScalar MAX_FRICTION  = 10.f;
	if (friction < -MAX_FRICTION)
		friction = -MAX_FRICTION;
	if (friction > MAX_FRICTION)
		friction = MAX_FRICTION;*/
	return friction;
	
}

inline btScalar	calculateCombinedRestitution(float restitution0,float restitution1)
{
	return restitution0 * restitution1;
}


static bool CustomMaterialCombinerCallback(btManifoldPoint& cp,	const btCollisionObject* colObj0,int partId0,int index0,const btCollisionObject* colObj1,int partId1,int index1)
{
	
	float friction0 = colObj0->getFriction();
	float friction1 = colObj1->getFriction();
	float restitution0 = colObj0->getRestitution();
	float restitution1 = colObj1->getRestitution();
	
	RudePhysicsObject *obj0 = RudePhysics::GetInstance()->GetObject(colObj0);
	RudePhysicsObject *obj1 = RudePhysics::GetInstance()->GetObject(colObj1);
	
	
	if (colObj0->getCollisionFlags() & btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK)
	{
		RUDE_ASSERT(obj0, "Custom callback but no object registered");
		RUDE_ASSERT(obj0->GetNotifyOnContact(), "No custom callback registered for object");
		
		obj0->Contact(obj1, partId0, partId1, &friction0, &restitution0);
	}
	if (colObj1->getCollisionFlags() & btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK)
	{
		RUDE_ASSERT(obj1, "Custom callback but no object registered");
		RUDE_ASSERT(obj1->GetNotifyOnContact(), "No custom callback registered for object");
		
		obj1->Contact(obj0, partId1, partId0, &friction1, &restitution1);
	}
	
	cp.m_combinedFriction = calculateCombinedFriction(friction0,friction1);
	cp.m_combinedRestitution = calculateCombinedRestitution(restitution0,restitution1);
	
	//printf("Friction of impact: %f\n", cp.m_combinedFriction);
	//printf("Restitution of impact: %f\n", cp.m_combinedRestitution);
	
	//this return value is currently ignored, but to be on the safe side: return false if you don't calculate friction
	return true;
}



RudePhysics::RudePhysics()
: m_dynamicsWorld(0)
{
}

RudePhysics::~RudePhysics()
{
	if(m_dynamicsWorld)
		delete m_dynamicsWorld;
}


RudePhysics * RudePhysics::GetInstance()
{
	static RudePhysics * s_singleton = 0;
	
	if(s_singleton == 0)
		s_singleton = new RudePhysics();
	
	return s_singleton;
}


void RudePhysics::Init()
{
	if(m_dynamicsWorld)
		return;
	
	
	gContactAddedCallback = CustomMaterialCombinerCallback;
	
	int maxProxies = 1024;
	
	btVector3 worldAabbMin(-10000,-10000,-10000);
	btVector3 worldAabbMax(10000,10000,10000);
	
	btAxisSweep3* broadphase = new btAxisSweep3(worldAabbMin,worldAabbMax,maxProxies);
	
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
	
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
	
	m_dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
	//m_dynamicsWorld = new btContinuousDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
	
	m_dynamicsWorld->setGravity(btVector3(0,-32.2,0));
	
	/*
	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),1);
	
	btDefaultMotionState* groundMotionState =
		new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-1,0)));
	
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
	btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
	groundRigidBody->setRestitution(1.0f);
	groundRigidBody->setCollisionFlags(groundRigidBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
	
	
	m_dynamicsWorld->addRigidBody(groundRigidBody);
	*/
	

}

void RudePhysics::AddObject(RudePhysicsObject *obj)
{
	btRigidBody *btrb = obj->GetRigidBody();
	RUDE_ASSERT(btrb, "Object has no rigid body");
	
	m_dynamicsWorld->addRigidBody(btrb);
	
	m_objMap[btrb] = obj;

}

void RudePhysics::NextFrame(float delta)
{
	if(m_dynamicsWorld)
		m_dynamicsWorld->stepSimulation(delta,20, 1.0f/60.0f);
	
}

