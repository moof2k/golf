/*
 *  RBScoreControl.cpp
 *  golf
 *
 *  Created by Robert Rose on 10/19/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "RBScoreControl.h"
#include "RBScoreTracker.h"
#include "RudeFont.h"

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>

RBScoreControl::RBScoreControl()
: m_activeHole(-1)
{
}


const int kLineSpacing = 16;
const int kColumnSpacing = 27;

void RBScoreControl::RenderScoreBoard(int x, int y, int start)
{
	RBScoreTracker *trackers[4];
	trackers[0] = GetScoreTracker(0);
	trackers[1] = GetScoreTracker(1);
	trackers[2] = GetScoreTracker(2);
	trackers[3] = GetScoreTracker(3);
	
	glDisable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	GLfloat point[] = {
		x - 5, y - 15,
		x - 5, y + 55,
		x + 300, y + 55,
		x + 300, y - 15
	};
	
	GLfloat colors[] = {
		1.0f, 1.0f, 1.0f, 0.2f,
		1.0f, 1.0f, 1.0f, 0.2f,
		1.0f, 1.0f, 1.0f, 0.2f,
		1.0f, 1.0f, 1.0f, 0.2f,
	};
	
	
	glVertexPointer(2, GL_FLOAT, 0, point);
	glColorPointer(4, GL_FLOAT, 0, colors);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	
	
	
	Printf(x, y + kLineSpacing, FONT_ALIGN_LEFT, kScoreColorNormal, "Par", 0);
	Printf(x, y + kLineSpacing * 2.0f, FONT_ALIGN_LEFT, kScoreColorNormal, "Strokes", 0);
	Printf(x, y + kLineSpacing * 3.0f, FONT_ALIGN_LEFT, kScoreColorNormal, "Score", 0);
	
	float xoff = x + 70;
	float yoff = y;
	
	for(int i = start; i < start + 9; i++)
	{
		Printf(xoff, yoff, FONT_ALIGN_CENTER, kScoreColorHole, "%d", i+1);
		
		eRBScoreColor color = kScoreColorNormal;
		
		if(i == m_activeHole)
			color = kScoreColorActiveHole;
		
		int par = trackers[0]->GetPar(i);
		int strokes = trackers[0]->GetNumStrokes(i);
		
		Printf(xoff, yoff + kLineSpacing, FONT_ALIGN_CENTER, color, "%d", par);
		
		if(strokes > 0)
		{
			Printf(xoff, yoff + kLineSpacing * 2.0f, FONT_ALIGN_CENTER, color, "%d", strokes);
			
			int score = strokes - par;
			
			if(score > 9)
				score = 9;
			
			if(score == 0)
				Printf(xoff, yoff + kLineSpacing * 3.0f, FONT_ALIGN_CENTER, color, "±0", score);
			else if(score > 0)
				Printf(xoff, yoff + kLineSpacing * 3.0f, FONT_ALIGN_CENTER, color, "+%d", score);
			else
				Printf(xoff, yoff + kLineSpacing * 3.0f, FONT_ALIGN_CENTER, color, "%d", score);
		}
		else
		{
			Printf(xoff, yoff + kLineSpacing * 2.0f, FONT_ALIGN_CENTER, color, "-", 0);
		}
					 
		xoff += kColumnSpacing;
	}
}

void RBScoreControl::Printf(float x, float y, int align, eRBScoreColor color, const char *fmt, int value)
{
	RudeFont *fontp = RudeFontManager::GetFont(kDefaultFont);
	RudeFont *fonto = RudeFontManager::GetFont(kDefaultFontOutline);
	
	unsigned int topcolor = 0xFFFFFFFF;
	unsigned int botcolor = 0xFFCCCCCC;
	
	if(color == kScoreColorHole)
	{
		topcolor = 0xFFFFFFFF;
		botcolor = 0xFFF69729;
	}
	else if(color == kScoreColorActiveHole)
	{
		topcolor = 0xFF0000FF;
		botcolor = 0xFFFFFFFF;
	}
	
	
	fonto->Printf(x, y, 0.0f, align, 0xFF222222, 0xFF000000, fmt, value);
	fontp->Printf(x, y, 0.0f, align, topcolor, botcolor, fmt, value);
}


void RBScoreControl::Render()
{
	int centery = (m_rect.m_bottom - m_rect.m_top) / 2;
	
	switch(m_holeSet)
	{
		case kCourseAll18:
			RenderScoreBoard(13, centery - 80, 0);
			RenderScoreBoard(13, centery + 20, 9);
			break;
		case kCourseFront9:
			RenderScoreBoard(13, centery, 0);
			break;
		case kCourseBack9:
			RenderScoreBoard(13, centery, 9);
			break;
	}
}


