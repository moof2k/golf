/*
 *  RBWindControl.h
 *  golf
 *
 *  Created by Robert Rose on 10/30/08.
 *  Copyright 2008 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef __H_RBWindControl
#define __H_RBWindControl

#include "RudeControl.h"
#include "RudeObject.h"

class RBWindControl : public RudeControl {
public:
	RBWindControl(RudeControl *parent);
	
	void NextFrame(float delta);
	void Render();
	
	void SetWind(float windDir, float windSpeed);
	
private:
	
	RudeObject m_windObject;
	
	btVector3 m_windVec;
	float m_windSpeed;
	
	float m_animTimer;
	float m_indicatorYaw;
	float m_indicatorYawTwitch;
	bool m_twitchUp;
};

#endif
