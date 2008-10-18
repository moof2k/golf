/*
 *  RudePhysics.h
 *  golf
 *
 *  Created by Robert Rose on 9/8/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __H_RudePhysics
#define __H_RudePhysics

#include <map>
#include <btBulletDynamicsCommon.h>

class RudePhysicsObject;

class RudePhysics {

public:
	RudePhysics();
	~RudePhysics();
	
	static RudePhysics * GetInstance();
	
	void Init();
	void Destroy();

	void NextFrame(float delta);
	
	btDiscreteDynamicsWorld * GetWorld() { return m_dynamicsWorld; }
	
	void AddObject(RudePhysicsObject *obj);
	
	RudePhysicsObject * GetObject(const btCollisionObject *cobj) { return m_objMap[cobj]; }
	
private:
	
	std::map<const btCollisionObject *, RudePhysicsObject *> m_objMap;
	
	btDiscreteDynamicsWorld *m_dynamicsWorld;
	btAxisSweep3* m_broadphase;
	btDefaultCollisionConfiguration* m_collisionConfiguration;
	btCollisionDispatcher* m_dispatcher;
	btSequentialImpulseConstraintSolver* m_solver;
};

const int kMaxRayResults = 16;

struct	RudeRayQueryResultCallback : public btCollisionWorld::RayResultCallback
{
	RudeRayQueryResultCallback(const btVector3&	rayFromWorld, const btVector3&	rayToWorld)
	:m_rayFromWorld(rayFromWorld),
	m_rayToWorld(rayToWorld),
	m_numRayResults(0)
	{
	}
	
	
	btCollisionObject * m_rayResults[kMaxRayResults];
	int m_numRayResults;
	
	btVector3	m_rayFromWorld;//used to calculate hitPointWorld from hitFraction
	btVector3	m_rayToWorld;
	
	btVector3	m_hitNormalWorld;
	btVector3	m_hitPointWorld;
	
	virtual	btScalar	addSingleResult(btCollisionWorld::LocalRayResult& rayResult,bool normalInWorldSpace)
	{
		m_rayResults[m_numRayResults] = rayResult.m_collisionObject;
		m_numRayResults++;
		
		//caller already does the filter on the m_closestHitFraction
		btAssert(rayResult.m_hitFraction <= m_closestHitFraction);
		
		m_closestHitFraction = rayResult.m_hitFraction;
		m_collisionObject = rayResult.m_collisionObject;
		if (normalInWorldSpace)
		{
			m_hitNormalWorld = rayResult.m_hitNormalLocal;
		} else
		{
			///need to transform normal into worldspace
			m_hitNormalWorld = m_collisionObject->getWorldTransform().getBasis()*rayResult.m_hitNormalLocal;
		}
		m_hitPointWorld.setInterpolate3(m_rayFromWorld,m_rayToWorld,rayResult.m_hitFraction);
		return rayResult.m_hitFraction;
	}
};


#endif

