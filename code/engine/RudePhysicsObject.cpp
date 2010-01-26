/*
 *	RudePhysicsObject.cpp
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#include "RudePhysicsObject.h"



#include <btBulletDynamicsCommon.h>

RudePhysicsObject::RudePhysicsObject(RudeObject *owner)
: m_motionState(0)
, m_rigidBody(0)
, m_notifyOnContact(false)
, m_contactCallback(0)
, m_owner(owner)
{
}

RudePhysicsObject::~RudePhysicsObject()
{
	if(m_rigidBody)
		delete m_rigidBody;
	if(m_motionState)
		delete m_motionState;
}


void RudePhysicsObject::Render()
{
	btTransform trans;
	m_rigidBody->getMotionState()->getWorldTransform(trans);
	btScalar m[16];
	trans.getOpenGLMatrix(m);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(m);
}

void RudePhysicsObject::Contact(const btVector3 &contactNormal, RudePhysicsObject *other, int mypartId, int otherpartId, float *friction, float *restitution)
{
	if(!m_notifyOnContact)
		return;
	
	if(!m_contactCallback)
		return;
	
	m_contactCallback(contactNormal, this, other, mypartId, otherpartId, friction, restitution);
}

