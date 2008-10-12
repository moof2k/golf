/*
 *  RudePhysicsObject.h
 *  golf
 *
 *  Created by Robert Rose on 9/8/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

class btDefaultMotionState;
class btRigidBody;
class RudeObject;
class RudePhysicsObject;

typedef void(*RudePhysicsCollisionCallback)(RudePhysicsObject *, RudePhysicsObject *, int, int, float *, float *);

class RudePhysicsObject {

public:
	RudePhysicsObject(RudeObject *owner);
	virtual ~RudePhysicsObject();
	
	void Render();
	
	btRigidBody * GetRigidBody() { return m_rigidBody; }
	
	void Contact(RudePhysicsObject *other, int mypartId, int otherpartId, float *friction, float *restitution);
	
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