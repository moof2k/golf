/*
 *  RudePhysicsObject.h
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#pragma once

#include "Rude.h"
#include <btBulletDynamicsCommon.h>


class RudeObject;
class RudePhysicsObject;

typedef void(*RudePhysicsCollisionCallback)(const btVector3 &contactNormal, RudePhysicsObject *, RudePhysicsObject *, int, int, float *, float *);

class RudePhysicsObject {

public:
	RudePhysicsObject(RudeObject *owner);
	virtual ~RudePhysicsObject();
	
	void Render();
	
	btRigidBody * GetRigidBody() { return m_rigidBody; }
	
	void Contact(const btVector3 &contactNormal, RudePhysicsObject *other, int mypartId, int otherpartId, float *friction, float *restitution);
	
	void SetContactCallback(RudePhysicsCollisionCallback cb) { m_contactCallback = cb; }
	bool GetNotifyOnContact() { return m_notifyOnContact; }
	void SetNotifyOnContact(bool b) { m_notifyOnContact = b; }
	
	RudeObject * GetOwner() { return m_owner; }
	
protected:
	RudeObject *m_owner;
	
	btDefaultMotionState *m_motionState;
	btRigidBody *m_rigidBody;
	
	bool m_notifyOnContact;
	RudePhysicsCollisionCallback m_contactCallback;
	
private:

};