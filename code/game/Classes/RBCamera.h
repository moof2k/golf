/*
 *  RBCamera.h
 *  golf
 *
 *  Created by Robert Rose on 9/13/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __H_RBCamera
#define __H_RBCamera

#include "btVector3.h"

class RBCamera {

public:
	
	RBCamera();
	
	virtual void NextFrame(float delta) = 0;
	
	virtual void SetView(float aspect);
	
	void SetPos(const btVector3 &p) { m_pos = p; }
	
	virtual btVector3 & GetPos() { return m_pos; }
	virtual btVector3 & GetLookAt() { return m_lookAt; }
	virtual btVector3 & GetUp() { return m_up; }
	
protected:
	
	btVector3 m_pos;
	btVector3 m_lookAt;
	btVector3 m_up;
	
	float m_halffov;
};


#endif

