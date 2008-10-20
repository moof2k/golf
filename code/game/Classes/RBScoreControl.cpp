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
		Printf(xoff, yoff, FONT_ALIGN_CENTER, kScoreColorNormal, "%d", i+1);
		
		int par = trackers[0]->GetPar(i);
		int strokes = trackers[0]->GetNumStrokes(i);
		
		Printf(xoff, yoff + kLineSpacing, FONT_ALIGN_CENTER, kScoreColorNormal, "%d", par);
		
		if(strokes > 0)
		{
			Printf(xoff, yoff + kLineSpacing * 2.0f, FONT_ALIGN_CENTER, kScoreColorNormal, "%d", strokes);
			
			int score = trackers[0]->GetScore(i, true);
			
			if(score == 0)
				Printf(xoff, yoff + kLineSpacing * 3.0f, FONT_ALIGN_CENTER, kScoreColorNormal, "±0", score);
			else if(score > 0)
				Printf(xoff, yoff + kLineSpacing * 3.0f, FONT_ALIGN_CENTER, kScoreColorNormal, "+%d", score);
			else
				Printf(xoff, yoff + kLineSpacing * 3.0f, FONT_ALIGN_CENTER, kScoreColorNormal, "-0", score);
		}
		else
		{
			Printf(xoff, yoff + kLineSpacing * 2.0f, FONT_ALIGN_CENTER, kScoreColorNormal, "-", 0);
		}
					 
		xoff += kColumnSpacing;
	}
}

void RBScoreControl::Printf(float x, float y, int align, eRBScoreColor color, const char *fmt, int value)
{
	RudeFont *fontp = RudeFontManager::GetFont(kDefaultFont);
	RudeFont *fonto = RudeFontManager::GetFont(kDefaultFontOutline);
	
	fonto->Printf(x, y, 0.0f, align, 0xFF222222, 0xFF000000, fmt, value);
	fontp->Printf(x, y, 0.0f, align, 0xFFFFFFFF, 0xFFCCCCCC, fmt, value);
}


void RBScoreControl::Render()
{
	int centery = (m_rect.m_bottom - m_rect.m_top) / 2;
	
	
	RenderScoreBoard(13, centery - 80, 0);
	RenderScoreBoard(13, centery + 20, 9);
}


