/*
 *  RBGolfBall.h
 *  golf
 *
 *  Created by Robert Rose on 9/9/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __H_RBGolfBall
#define __H_RBGolfBall

#include "RudeObject.h"
#include "RBTerrainMaterial.h"

#include <btBulletDynamicsCommon.h>

class RBGolfBall : public RudeObject {
	
public:
	RBGolfBall();
	~RBGolfBall() {}
	
	void Load(const char *name);
	
	void NextFrame(float delta);
	
	void Render();
	void Render(btVector3 pos, btVector3 rot);
	
	void AddContactDamping(float linear, float angular)
	{
		m_inContact = 2;
		m_linearContactDamping = linear;
		m_angularContactDamping = angular;
	}
	
	btVector3 GetPosition();
	btVector3 GetAngularVelocity();
	btVector3 GetLinearVelocity();
	void SetPosition(const btVector3 &p);
	void SetForce(const btVector3 &f);
	
	void StickAtPosition(const btVector3 &p);
	
	void Stop() { m_stop = true; }
	bool GetStopped() { return m_stopped; }
	
	float GetBallScale() { return m_ballScale; }
	
	void SetCurMaterial(eRBTerrainMaterial m) { m_curMaterial = m; }
	eRBTerrainMaterial GetCurMaterial() { return m_curMaterial; }
	
private:
	
	int m_inContact;
	float m_linearContactDamping;
	float m_angularContactDamping;
	float m_curLinearDamping;
	float m_curAngularDamping;
	float m_movementThreshold;
	
	float m_ballScale;
	
	bool m_stop;
	bool m_stopped;
	
	eRBTerrainMaterial m_curMaterial;
	
};

#endif
