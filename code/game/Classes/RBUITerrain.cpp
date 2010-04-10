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
: m_mode(kTerrainModeNone)
{
	m_touch[0] = 0;
	m_touch[1] = 0;

	if(RUDE_IPAD)
	{
		m_tvc.SetRect(RudeRect(0,0,1024,768));
	}
	else
	{
		m_tvc.SetRect(RudeRect(0,0,480,320));
	}
}

RBUITerrain::~RBUITerrain()
{
}

void RBUITerrain::StartDisplay()
{
	m_done = false;
	m_mode = kTerrainModeNone;
	m_touch[0] = 0;
	m_touch[1] = 0;
}


void RBUITerrain::NextFrame(float delta)
{
	if(m_done)
		return;
	
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
			RudeScreenVertex pdiff = m_touch[0]->m_location - m_touch[1]->m_location;
			
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
			m_touch[1] = 0;
		break;
	}
}
