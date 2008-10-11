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

class btDiscreteDynamicsWorld;
class btCollisionObject;
class RudePhysicsObject;

class RudePhysics {

public:
	RudePhysics();
	~RudePhysics();
	
	static RudePhysics * GetInstance();
	
	void Init();

	void NextFrame(float delta);
	
	btDiscreteDynamicsWorld * GetWorld() { return m_dynamicsWorld; }
	
	void AddObject(RudePhysicsObject *obj);
	
	RudePhysicsObject * GetObject(const btCollisionObject *cobj) { return m_objMap[cobj]; }
	
private:
	
	std::map<const btCollisionObject *, RudePhysicsObject *> m_objMap;
	
	btDiscreteDynamicsWorld *m_dynamicsWorld;
};


#endif

