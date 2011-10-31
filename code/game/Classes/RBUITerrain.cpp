/*
 *  RBUITerrain.cpp
 *  golf
 *
 *  Created by Robert Rose on 4/30/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "RBUITerrain.h"

#include "RudeGL.h"
#include "RudeFont.h"
#include "RudeSound.h"
#include "RudeTextureManager.h"

RBUITerrain::RBUITerrain()
: m_tvc(0)
, m_mode(kTerrainModeNone)
, m_startingTouchDist(1.0f)
, m_touchDist(1.0f)
{
	m_touch[0] = 0;
	m_touch[1] = 0;

	m_tvc.SetFileRect(RudeRect(0,0,-1,-1));
}

RBUITerrain::~RBUITerrain()
{
}

void RBUITerrain::StartDisplay()
{
	m_tvc.Scale(1.0f);
	
	m_done = false;
	m_mode = kTerrainModeNone;
	m_touch[0] = 0;
	m_touch[1] = 0;
}


void RBUITerrain::NextFrame(float delta)
{
	if(m_done)
		return;
	
	m_tvc.NextFrame(delta);
}

void RBUITerrain::Render(float width, float height)
{
	if(m_done)
		return;
	
	m_tvc.Render();
}

void RBUITerrain::TouchDown(RudeTouch *rbt)
{
	if(m_done)
		return;

	switch(m_mode)
	{
		case kTerrainModeNone:
			m_mode = kTerrainModeTranslate;
			m_touch[0] = rbt;
			m_pos[0] = rbt->m_location;
			break;
		case kTerrainModeTranslate:
			m_touch[1] = rbt;
			m_mode = kTerrainModeScale;
			CalcTouchDist();
			m_startingTouchDist = m_touchDist;
			break;
		case kTerrainModeScale:

			break;
	}
	
}

void RBUITerrain::TouchMove(RudeTouch *rbt)
{
	if(rbt != m_touch[0] && rbt != m_touch[1])
		return;

	switch(m_mode)
	{
	case kTerrainModeNone:
		break;
	case kTerrainModeTranslate:
		{
			RudeScreenVertex diff = m_pos[0] - rbt->m_location;
			m_tvc.Translate(diff.m_x, diff.m_y);

			m_pos[0] = rbt->m_location;
		}
		break;
	case kTerrainModeScale:
		{
			CalcTouchDist();
			
			m_tvc.Scale(m_startingTouchDist - m_touchDist);
			m_startingTouchDist = m_touchDist;
		}
		break;
	}
}

void RBUITerrain::TouchUp(RudeTouch *rbt)
{
	switch(m_mode)
	{
	case kTerrainModeNone:
		break;
	case kTerrainModeTranslate:
		m_mode = kTerrainModeNone;

		if(rbt == m_touch[0])
			m_touch[0] = 0;
		break;
	case kTerrainModeScale:
		m_mode = kTerrainModeTranslate;

		if(rbt == m_touch[1])
		{
			m_touch[1] = 0;
		}
		else if(rbt == m_touch[0])
		{
			m_touch[0] = m_touch[1];
			m_touch[1] = 0;
			m_pos[0] = m_touch[0]->m_location;
		}
		break;
	}
}

void RBUITerrain::CalcTouchDist()
{
	if(m_touch[0] == 0)
		return;
	if(m_touch[1] == 0)
		return;
	
	RudeScreenVertex pdiff = m_touch[0]->m_location - m_touch[1]->m_location;
	m_touchDist = pdiff.m_x * pdiff.m_x + pdiff.m_y * pdiff.m_y;
	if(m_touchDist > 0.0f)
		m_touchDist = sqrt(m_touchDist);
}

void RBUITerrain::Resize()
{
	m_tvc.UpdateDrawRect();
}
