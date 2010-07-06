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
	
	
	
#if RUDE_IPAD == 1
	const float baseY = 45;

	const float courseX = 290;
	const float courseY = baseY;

	const float holesX = 290;
	const float holesY = baseY + 27;

	const float scoreX = 230;
	const float scoreY = baseY + 14;
#else
	const float baseY = 23;

	const float courseX = 100;
	const float courseY = baseY;

	const float holesX = 100;
	const float holesY = baseY + 19;

	const float scoreX = 61;
	const float scoreY = baseY + 9;
#endif

	
	RudeFontManager::GetFont(kDefaultFontOutline)->Write(x + courseX, y + courseY, 0.0f, m_name, 0, FONT_ALIGN_LEFT, 0xFFFFFFFF, 0xFFCCCCCC);
	RudeFontManager::GetFont(kDefaultFont)->Write(x + courseX, y + courseY, 0.0f, m_name, 0, FONT_ALIGN_LEFT, 0xFF444444, 0xFF000000);
	
	
	RudeFontManager::GetFont(kDefaultFontOutline)->Write(x + holesX, y + holesY, 0.0f, m_subname, 0, FONT_ALIGN_LEFT, 0xFFFFFFFF, 0xFFCCCCCC);
	RudeFontManager::GetFont(kDefaultFont)->Write(x + holesX, y + holesY, 0.0f, m_subname, 0, FONT_ALIGN_LEFT, 0xFF444444, 0xFF000000);

	
	
	unsigned int scorecolor = 0xFF000000;
	
	if(!m_completed)
		scorecolor = 0xFF0000FF;
	
	RudeFontManager::GetFont(kDefaultFontOutline)->Write(x + scoreX, y + scoreY, 0.0f, m_scoreText, 0, FONT_ALIGN_LEFT, 0xFFFFFFFF, 0xFFCCCCCC);
	RudeFontManager::GetFont(kDefaultFont)->Write(x + scoreX, y + scoreY, 0.0f, m_scoreText, 0, FONT_ALIGN_LEFT, 0xFF444444, scorecolor);
	
	
	
	
	/*
	const char *holestr = GetHoleStr();
	
	const float holesX = 100;
	const float holesY = 44;
	
	RudeFontManager::GetFont(kDefaultFontOutline)->Write(x + holesX, y + holesY, 0.0f, holestr, 0, FONT_ALIGN_LEFT, 0xFFFFFFFF, 0xFFCCCCCC);
	RudeFontManager::GetFont(kDefaultFont)->Write(x + holesX, y + holesY, 0.0f, holestr, 0, FONT_ALIGN_LEFT, 0xFF444444, 0xFF000000);

	const char *teestr = GetTeeStr();

	const float teeX = 200;
	const float teeY = 44;
	
	RudeFontManager::GetFont(kDefaultFontOutline)->Write(x + teeX, y + teeY, 0.0f, teestr, 0, FONT_ALIGN_LEFT, 0xFFFFFFFF, 0xFFCCCCCC);
	RudeFontManager::GetFont(kDefaultFont)->Write(x + teeX, y + teeY, 0.0f, teestr, 0, FONT_ALIGN_LEFT, 0xFF444444, 0xFF000000);
	 */
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
