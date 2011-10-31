/*
 *  RBScoreControl.cpp
 *  golf
 *
 *  Created by Robert Rose on 10/19/08.
 *  Copyright 2008 Bork 3D LLC. All rights reserved.
 *
 */

#include "RBScoreControl.h"
#include "RBScoreTracker.h"
#include "RudeFont.h"
#include "RudeGL.h"

RBScoreControl::RBScoreControl(RudeControl *parent)
: RudeControl(parent)
, m_activeHole(-1)
, m_holeSet(kCourseFront9)
{
}




#if RUDE_IPAD == 1
	const float kHalfWidth = 230.0f;
	const float kTop = 24.0f;
	const float kBottom = 85.0f;

	const int kLineSpacing = 26;
	const int kColumnSpacing = 38;

	const int kTextOffset = 220;
	const int kScoreOffset = 124;

	const int kScore1Offset = -120;
	const int kScore2Offset = 20;
#else
	const float kHalfWidth = 153.0f;
	const float kTop = 15.0f;
	const float kBottom = 55.0f;

	const int kLineSpacing = 16;
	const int kColumnSpacing = 27;

	const int kTextOffset = 147;
	const int kScoreOffset = 70;

	const int kScore1Offset = -80;
	const int kScore2Offset = 20;
#endif

void RBScoreControl::RenderScoreBoard(int x, int y, int start)
{
	RBScoreTracker *trackers[4];
	trackers[0] = GetScoreTracker(0);
	trackers[1] = GetScoreTracker(1);
	trackers[2] = GetScoreTracker(2);
	trackers[3] = GetScoreTracker(3);
	
	glDisable(GL_TEXTURE_2D);
	
	RGL.EnableClient(kVertexArray, true);
	RGL.EnableClient(kColorArray, true);
	RGL.EnableClient(kTextureCoordArray, false);
	
	GLfloat point[] = {
		x - kHalfWidth, y - kTop,
		x - kHalfWidth, y + kBottom,
		x + kHalfWidth, y + kBottom,
		x + kHalfWidth, y - kTop
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
	
	x -= kTextOffset;
	
	Printf(x, y + kLineSpacing, FONT_ALIGN_LEFT, kScoreColorNormal, "Par", 0);
	Printf(x, y + kLineSpacing * 2.0f, FONT_ALIGN_LEFT, kScoreColorNormal, "Strokes", 0);
	Printf(x, y + kLineSpacing * 3.0f, FONT_ALIGN_LEFT, kScoreColorNormal, "Score", 0);
	
	float xoff = x + kScoreOffset;
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
	RudeControl::Render();
	
	int centery = (m_drawRect.m_bottom - m_drawRect.m_top) / 2;

	// 295 pixels wide
	int centerx = (m_drawRect.m_right - m_drawRect.m_left) / 2;

	switch(m_holeSet)
	{
		case kCourseAll18:
			RenderScoreBoard(centerx, centery + kScore1Offset, 0);
			RenderScoreBoard(centerx, centery + kScore2Offset, 9);
			break;
		case kCourseFront9:
			RenderScoreBoard(centerx, centery, 0);
			break;
		case kCourseBack9:
			RenderScoreBoard(centerx, centery, 9);
			break;
	}
}


RudeControl * ConstructRBScoreControl(RudeControl *parent, std::list<std::string> &tokens, const std::string &originalDesc)
{
	RBScoreControl *c = new RBScoreControl(parent);
	RUDE_ASSERT(c, "Failed to construct control");

	// Rect {t,l,b,r}
	std::string rectstr = RudeControl::PopToken(tokens, originalDesc, "rect");

	RudeRect rect;
	RudeControl::ParseRect(rectstr, rect);
	c->SetFileRect(rect);

	return c;
}

RudeControlRegistration rbScoreRegistration("RBScoreControl", ConstructRBScoreControl);


