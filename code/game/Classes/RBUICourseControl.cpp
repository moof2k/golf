/*
 *  RBUICourseControl.cpp
 *  golf
 *
 *  Created by Robert Rose on 10/29/08.
 *  Copyright 2008 Bork 3D LLC. All rights reserved.
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

	const char *holestr = GetHoleStr();
	
	
	
	const float holesX = 100;
	const float holesY = 44;
	
	RudeFontManager::GetFont(kDefaultFont)->Write(x + holesX, y + holesY, 0.0f, holestr, 0, FONT_ALIGN_LEFT, 0xFF000000, 0xFF000000);

	const char *teestr = GetTeeStr();

	const float teeX = 200;
	const float teeY = 44;
	
	RudeFontManager::GetFont(kDefaultFont)->Write(x + teeX, y + teeY, 0.0f, teestr, 0, FONT_ALIGN_LEFT, 0xFF000000, 0xFF000000);

}


const char * RBUICourseControl::GetHoleStr()
{
	if(m_holes == kCourseFront9)
		return "Front 9";
	else if(m_holes == kCourseBack9)
		return "Back 9";
	else
		return "18 Holes";
}

const char * RBUICourseControl::GetTeeStr()
{
	if(m_tee == kCourseShortTee)
		return "Short Tee";
	else
		return "Long Tee";
	
}
