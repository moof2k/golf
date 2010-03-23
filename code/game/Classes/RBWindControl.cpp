/*
 *  RBWindControl.cpp
 *  golf
 *
 *  Created by Robert Rose on 10/30/08.
 *  Copyright 2008 Bork 3D LLC. All rights reserved.
 *
 */

#include "RBWindControl.h"
#include "RudeGL.h"
#include "RudeDebug.h"

RBWindControl::RBWindControl()
: m_windSpeed(0.0f)
, m_windVec(0,0,0)
, m_animTimer(0.0f)
, m_indicatorYaw(0.0f)
, m_indicatorYawTwitch(0.0f)
{
	m_windObject.LoadMesh("wind_indicator");
	
}

void RBWindControl::SetWind(float windDir, float windSpeed)
{
	m_indicatorYaw = windDir * 180.0f / 3.1415926f;
	m_windSpeed = windSpeed;
}


void RBWindControl::NextFrame(float delta)
{
	m_animTimer += delta;
	
	const float kTwitchMax = 8.0f;
	
	if(m_twitchUp)
	{
		m_indicatorYawTwitch += delta * m_windSpeed;
		if(m_indicatorYawTwitch > kTwitchMax)
		{
			m_indicatorYawTwitch = kTwitchMax;
			m_twitchUp = false;
		}
			
	}
	else
	{
		m_indicatorYawTwitch -= delta * m_windSpeed;
		if(m_indicatorYawTwitch < -kTwitchMax)
		{
			m_indicatorYawTwitch = -kTwitchMax;
			m_twitchUp = true;
		}
	}
}

void RBWindControl::Render()
{
	RGL.SetViewport(m_rect.m_top, m_rect.m_left, m_rect.m_bottom, m_rect.m_right);
	
	btVector3 eyevec = RGL.GetForward();
	eyevec.setY(0.0f);
	eyevec.normalize();
	eyevec.setY(1.0f);
	eyevec *= 5.0f;
	
	RGL.Frustum(0.0f, 0.0f, 0.2f, 0.2f, 1.0f, 10.0f);
	
	RGL.LookAt(eyevec.x(), eyevec.y(), eyevec.z(), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	
	RGL.LoadIdentity();
	
	RGL.Rotate(m_indicatorYaw + m_indicatorYawTwitch, 0.0f, 1.0f, 0.0f);
	
	
	m_windObject.Render();
}



/**
 * ConstructRBWindControl factory assistant for RudeControl.  This is called by RudeControl::Load()
 */
RudeControl * ConstructRBWindControl(std::list<std::string> &tokens, const std::string &originalDesc)
{
	RBWindControl *c = new RBWindControl();
	RUDE_ASSERT(c, "Failed to construct control");

	// Rect {t,l,b,r}
	std::string rectstr = RudeControl::PopToken(tokens, originalDesc, "rect");

	RudeRect rect;
	RudeControl::ParseRect(rectstr, rect);
	c->SetRect(rect);

	return c;
}

RudeControlRegistration rbWindControlRegistration("RBWindControl", ConstructRBWindControl);



