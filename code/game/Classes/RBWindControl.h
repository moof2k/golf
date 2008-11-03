/*
 *  RBWindControl.h
 *  golf
 *
 *  Created by Robert Rose on 10/30/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __H_RBWindControl
#define __H_RBWindControl

#include "RudeControl.h"

class RBWindControl : public RudeControl {
public:
	RBWindControl();
	
	void SetWind(const btVector3 &windVec, float windSpeed)
	{
		m_windVec = windVec;
		m_windSpeed = windSpeed;
	}
	
private:
	
	btVector3 m_windVec;
	float m_windSpeed;
};

#endif
