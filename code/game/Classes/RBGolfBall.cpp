/*
 *  RBGolfBall.cpp
 *  golf
 *
 *  Created by Robert Rose on 9/9/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "RBGolfBall.h"
#include "RudeGL.h"
#include "RudePhysicsSphere.h"
#include "RudePhysics.h"
#include "RudeMesh.h"
#include <btBulletDynamicsCommon.h>

const float kBallRadius = 0.15f;
const float kBallMass = 0.1f;
const float kSpinForceDelay = 0.75f;
const float kWindSpeedDelay = 1.0f;

RBGolfBall::RBGolfBall()
: m_linearContactDamping(0.0f)
, m_angularContactDamping(0.0f)
, m_movementThreshold(0.0f)
, m_curLinearDamping(0.0f)
, m_curAngularDamping(0.0f)
, m_inContact(false)
, m_stop(false)
, m_stopped(false)
, m_ballScale(kBallRadius)
, m_curMaterial(kTee)
, m_spinForce(0,0,0)
, m_spinForceTimer(0.0f)
, m_windSpeed(0,0,10)
, m_windSpeedTimer(0.0f)
{
}

void RBGolfBall::Load(const char *name)
{
	LoadMesh(name);
	LoadPhysicsSphere(kBallRadius, kBallMass);
	
	//RudeMesh *mesh = GetMesh();
	//mesh->SetScale(btVector3(kBallRadius, 0.2, 0.2));
	
	RudePhysicsSphere *obj = (RudePhysicsSphere *) GetPhysicsObject();
	btRigidBody *rb = obj->GetRigidBody();
	
	rb->setFriction(1.0f);
	rb->setRestitution(0.78f);
	
	rb->setCcdMotionThreshold(kBallRadius * 0.5f);
	rb->setCcdSweptSphereRadius(kBallRadius * 0.5f);
}


void RBGolfBall::NextFrame(float delta)
{
	RudePhysicsSphere *obj = (RudePhysicsSphere *) GetPhysicsObject();
	btRigidBody *rb = obj->GetRigidBody();
	
	// apply force from spin
	if(m_applySpinForce)
	{
		m_spinForceTimer += delta;
		
		float gradient = m_spinForceTimer / kSpinForceDelay;
		if(gradient > 1.0f)
			gradient = 1.0f;
		
		btVector3 linvel = rb->getLinearVelocity();
		linvel += m_spinForce * delta * gradient;
		rb->setLinearVelocity(linvel);
		
	}
	
	// apply wind
	if(m_inContact == 0)
	{
		m_windSpeedTimer += delta;
		
		float gradient = m_windSpeedTimer / kWindSpeedDelay;
		
		if(gradient > 1.0f)
			gradient = 1.0f;
		
		btVector3 linvel = rb->getLinearVelocity();
		linvel += m_windSpeed * delta * gradient;
		rb->setLinearVelocity(linvel);
	}
	else
		m_windSpeedTimer = 0.0f;
	
	if(m_inContact > 0)
	{
		if(m_applySpinForce)
		{
			if(m_spinForceTimer > 0.1f)
				m_applySpinForce = false;
		}
		
		m_curLinearDamping += m_linearContactDamping * delta;
		m_curAngularDamping += m_angularContactDamping * delta;
		
		rb->setDamping(m_curLinearDamping, m_curAngularDamping);
		
		m_inContact--;
	}
	else
	{
		m_curLinearDamping = 0.0f;
		m_curAngularDamping = 0.0f;
		m_angularContactDamping = 0.0f;
		m_linearContactDamping = 0.0f;
		rb->setDamping(0.0f, 0.0f);
	}
	
	if(m_stop)
	{
		GetPhysicsObject()->GetRigidBody()->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
		m_stopped = true;
		
	}
	
	//if(!rb->isActive())
	// ball has reached a stop state
	//	printf("INACTIVE\n");
	

	//printf("l = %f, a = %f\n", m_curLinearDamping, m_curAngularDamping);
}


void RBGolfBall::Render()
{
	btVector3 eye = RGL.GetEye();
	
	btTransform trans;
	GetPhysicsObject()->GetRigidBody()->getMotionState()->getWorldTransform(trans);
	//trans = GetPhysicsObject()->GetRigidBody()->getWorldTransform();
	btVector3 ballpos = trans.getOrigin();
	
	float scalefactor = 1.0f;
	float eyedist = (eye - ballpos).length();
	scalefactor += eyedist / 75.0f;
	
	m_ballScale = kBallRadius * scalefactor;
	
	GetPhysicsObject()->Render();
	
	glScalef(m_ballScale, m_ballScale, m_ballScale);
	
	GetMesh()->Render();
}

void RBGolfBall::Render(btVector3 pos, btVector3 rot)
{
	glMatrixMode(GL_MODELVIEW);
	
	btMatrix3x3 rotmat;
	
	if(rot.length() > 0.1f)
	{
		btQuaternion q(rot.normalize(), rot.length());
		rotmat.setRotation(q);
	}
	else
		rotmat.setIdentity();
	
	
	btTransform trans(rotmat, pos);
	
	btScalar m[16];
	trans.getOpenGLMatrix(m);
					   
	glLoadMatrixf(m);
	
	glScalef(kBallRadius, kBallRadius, kBallRadius);
	
	GetMesh()->Render();
}

btVector3 RBGolfBall::GetPosition()
{
	btTransform trans;
	GetPhysicsObject()->GetRigidBody()->getMotionState()->getWorldTransform(trans);
	
	return trans.getOrigin();
}

btVector3 RBGolfBall::GetAngularVelocity()
{
	return GetPhysicsObject()->GetRigidBody()->getAngularVelocity();
}

btVector3 RBGolfBall::GetLinearVelocity()
{
	return GetPhysicsObject()->GetRigidBody()->getLinearVelocity();
}

void RBGolfBall::SetPosition(const btVector3 &p)
{
	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(p);
	GetPhysicsObject()->GetRigidBody()->setWorldTransform(trans);
	GetPhysicsObject()->GetRigidBody()->activate(true);
}

void RBGolfBall::HitBall(const btVector3 &linvel, const btVector3 &spinForce)
{
	m_spinForce = spinForce;
	m_applySpinForce = true;
	m_spinForceTimer = 0.0f;
	
	m_windSpeedTimer = 0.0f;
	
	m_stop = false;
	m_stopped = false;
	
	GetPhysicsObject()->GetRigidBody()->setCollisionFlags(0);
	
	GetPhysicsObject()->GetRigidBody()->setLinearVelocity(linvel);
	GetPhysicsObject()->GetRigidBody()->setAngularVelocity(btVector3(0,0,0));
	GetPhysicsObject()->GetRigidBody()->clearForces();
	//GetPhysicsObject()->GetRigidBody()->applyForce(p, btVector3(0,0,0));
	GetPhysicsObject()->GetRigidBody()->activate(true);
	
	m_inContact = 0;
}

void RBGolfBall::StickAtPosition(const btVector3 &p)
{
	m_stopped = true;
	
	btVector3 to = p + btVector3(0,-1000,0);
	btDiscreteDynamicsWorld *world = RudePhysics::GetInstance()->GetWorld();
	btCollisionWorld::ClosestRayResultCallback cb(p, to);
	
	world->rayTest(p, to, cb);
	
	RUDE_ASSERT(cb.hasHit(), "Could not stick ball at position");
	
	
	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(cb.m_hitPointWorld + btVector3(0, 0.5f + kBallRadius,0));
	
	GetPhysicsObject()->GetRigidBody()->getMotionState()->setWorldTransform(trans);
	GetPhysicsObject()->GetRigidBody()->setWorldTransform(trans);
	
	GetPhysicsObject()->GetRigidBody()->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
}

