/*
 *  RBTRound.cpp
 *  golf
 *
 *  Created by Robert Rose on 10/17/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "RBTRound.h"
#include "RudeGL.h"

RBTRound::RBTRound()
: m_game(0)
, m_state(kStateInit)
{
	m_loadingText.SetAlignment(kAlignCenter);
	m_loadingText.SetRect(RudeRect(300, 0, 316, 320));
	m_loadingText.SetText("Loading...");
	m_loadingText.SetFont(kBigFont);
	m_loadingText.SetStyle(kOutlineStyle);
	m_loadingText.SetColors(0, 0xFFFFFFFF, 0xFFFFFFFF);
	m_loadingText.SetColors(1, 0xFF222222, 0xFF222222);
}

RBTRound::~RBTRound()
{
}

void RBTRound::NextFrame(float delta)
{
	if(m_state == kStateInit)
	{
		RUDE_REPORT("RBTRound State %d\n", m_state);
		
		m_state = kStateNextRound;
	}
	else if(m_state == kStateNextRound)
	{
		RUDE_REPORT("RBTRound State %d\n", m_state);
		
		if(m_game)
			delete m_game;
		
		static int s = 0;
		s++;
		m_game = new RBTGame("parfive", s);
		
		m_state = kStateInRound;
		 
	}
	else if(m_state == kStateInRound)
	{
		if(m_game)
		{
			m_game->NextFrame(delta);
			
			if(m_game->Done())
			{
				m_state = kStateNextRound;
			}
		}
	}
}

void RBTRound::Render(float aspect)
{
	if(m_state == kStateNextRound || m_state == kStateInit)
	{
		RGL.SetViewport(0, 0, 480-1, 320-1);
		RGL.Enable(kBackfaceCull, false);
		RGL.Enable(kDepthTest, false);
		RGL.Ortho(0.0f, 0.0f, 0.0f, 320.0f, 480.0f, 100.0f);
		RGL.LoadIdentity();


		m_loadingText.Render();
		
		
	}
	else if(m_state == kStateInRound)
	{
		if(m_game)
			m_game->Render(aspect);
	}
}

void RBTRound::TouchDown(RudeTouch *rbt)
{
	if(m_game)
		m_game->TouchDown(rbt);
}

void RBTRound::TouchMove(RudeTouch *rbt)
{
	if(m_game)
		m_game->TouchMove(rbt);
}

void RBTRound::TouchUp(RudeTouch *rbt)
{
	if(m_game)
		m_game->TouchUp(rbt);
}
