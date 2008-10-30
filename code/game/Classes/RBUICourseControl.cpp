/*
 *  RBUICourseControl.cpp
 *  golf
 *
 *  Created by Robert Rose on 10/29/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "RBUICourseControl.h"
#include "RudeFont.h"
#include "RudeGL.h"

void RBUICourseControl::Render()
{
	RudeButtonControl::Render();
	
	float x = m_rect.m_left;
	float y = m_rect.m_top;
	
	const float courseX = 100;
	const float courseY = 27;
	
	RudeFontManager::GetFont(kDefaultFont)->Write(x + courseX, y + courseY, 0.0f, m_name, 0, FONT_ALIGN_LEFT, 0xFF000000, 0xFF000000);

	const char *holestr = "";
	
	if(m_holes == kCourseFront9)
		holestr = "Front 9";
	else if(m_holes == kCourseBack9)
		holestr = "Back 9";
	else if(m_holes	== kCourseAll18)
		holestr = "18 Holes";
	
	const float holesX = 100;
	const float holesY = 44;
	
	RudeFontManager::GetFont(kDefaultFont)->Write(x + holesX, y + holesY, 0.0f, holestr, 0, FONT_ALIGN_LEFT, 0xFF000000, 0xFF000000);

	const char *teestr = "";
	
	if(m_tee == kCourseShortTee)
		teestr = "Short Tee";
	else
		teestr = "Long Tee";

	const float teeX = 200;
	const float teeY = 44;
	
	RudeFontManager::GetFont(kDefaultFont)->Write(x + teeX, y + teeY, 0.0f, teestr, 0, FONT_ALIGN_LEFT, 0xFF000000, 0xFF000000);

}