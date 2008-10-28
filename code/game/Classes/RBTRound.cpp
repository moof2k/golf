/*
 *  RBTRound.cpp
 *  golf
 *
 *  Created by Robert Rose on 10/17/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "RBTRound.h"
#include "RBScoreControl.h"
#include "RudeGL.h"

static RBTHole sCourse[18] = {
	{ 1, 3, "par3_C"},
	{ 2, 3, "par3_B" },
	{ 3, 3, "par3_C" },
	{ 4, 3, "par3_D" },
	{ 5, 3, "par3_E" },
	{ 6, 5, "parfive" },
	{ 7, 5, "parfive" },
	{ 8, 5, "parfive" },
	{ 9, 5, "parfive" },
	{ 10, 5, "parfive" },
	{ 11, 5, "parfive" },
	{ 12, 5, "parfive" },
	{ 13, 5, "parfive" },
	{ 14, 5, "parfive" },
	{ 15, 5, "parfive" },
	{ 16, 5, "parfive" },
	{ 17, 5, "parfive" },
	{ 18, 5, "parfive" }

};

RBTRound::RBTRound()
: m_game(0)
, m_state(kStateInit)
, m_hole(0)
, m_numPlayers(1)
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
		
		m_hole = 0;
		
		for(int i = 0; i < m_numPlayers; i++)
		{
			RBScoreTracker *tracker = GetScoreTracker(i);
			for(int h = 0; h < 18; h++)
			{
				tracker->ClearScores();
				tracker->SetPar(h, sCourse[h].m_par);
			}
		}
		
		m_state = kStateNextRound;
	}
	else if(m_state == kStateNextRound)
	{
		RUDE_REPORT("RBTRound State %d, Hole %d\n", m_state, m_hole);
		
		if(m_game)
			delete m_game;
		
		m_game = new RBTGame(m_hole, sCourse[m_hole].m_terrainFile, sCourse[m_hole].m_par, m_numPlayers);
		
		m_state = kStateInRound;
		 
	}
	else if(m_state == kStateInRound)
	{
		if(m_game)
		{
			m_game->NextFrame(delta);
			
			if(m_game->Done())
			{
				
				
				m_hole++;
				m_state = kStateNextRound;
			}
		}
	}
}

void RBTRound::Render(float aspect)
{
	if(m_state == kStateNextRound || m_state == kStateInit)
	{
		RGL.SetViewport(0, 0, 480, 320);
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
