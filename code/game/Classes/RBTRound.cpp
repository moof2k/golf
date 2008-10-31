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
#include "RBCourseData.h"


RBTRound::RBTRound()
: m_game(0)
, m_state(kStateInit)
, m_hole(0)
, m_holeSet(kCourseFront9)
, m_numPlayers(1)
, m_tee(kCourseShortTee)
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
	if(m_game)
	{
		delete m_game;
		m_game = 0;
	}
}

void RBTRound::Reset()
{
	if(m_game)
	{
		delete m_game;
		m_game = 0;
	}
	
	m_state = kStateInit;
}

void RBTRound::SetCourse(int c)
{
	RBCourseEntry *cd = GetCourseData(c);
	
	m_holeSet = cd->m_holes;
	m_tee = cd->m_tee;
}

void RBTRound::NextFrame(float delta)
{
	if(m_state == kStateInit)
	{
		RUDE_REPORT("RBTRound State %d\n", m_state);
		
		m_hole = 0;
		m_done = false;
		
		for(int i = 0; i < m_numPlayers; i++)
		{
			RBScoreTracker *tracker = GetScoreTracker(i);
			for(int h = 0; h < 18; h++)
			{
				RBCourseHole *hole = GetCourseHole(0, kCourseAll18, h);
				tracker->ClearScores();
				tracker->SetPar(h, hole->m_par);
			}
		}
		
		m_state = kStateNextRound;
	}
	else if(m_state == kStateNextRound)
	{
		RUDE_REPORT("RBTRound State %d, Hole %d\n", m_state, m_hole);
		
		if(m_game)
		{
			delete m_game;
			m_game = 0;
		}
		
		RBCourseHole *hole = GetCourseHole(0, m_holeSet, m_hole);
		
		if(hole)
		{
			m_game = new RBTGame(m_hole, hole->m_terrainFile, m_tee, m_holeSet, hole->m_par, m_numPlayers);
			m_state = kStateInRound;
		}
		else
		{
			m_state = kStateDone;
		}
		 
	}
	else if(m_state == kStateInRound)
	{
		if(m_game)
		{
			m_game->NextFrame(delta);
			
			if(m_game->Done())
			{
				if(m_game->GetResult() == kResultComplete)
				{
					m_hole++;
					m_state = kStateNextRound;
				}
				else if(m_game->GetResult() == kResultQuit)
				{
					m_state = kStateDone;
				}
			}
		}
	}
	else if(m_state == kStateDone)
	{
		if(m_game)
		{
			delete m_game;
			m_game = 0;
		}
		
		m_done = true;
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
