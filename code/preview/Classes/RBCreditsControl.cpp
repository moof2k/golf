/*
 *  RBCreditsControl.cpp
 *  golf
 *
 *  Created by Robert Rose on 4/25/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "RBCreditsControl.h"

#include "RudeFont.h"

RBCreditsControl::RBCreditsControl()
{
}


void RBCreditsControl::Render()
{
	RudeControl::Render();
	
	//RudeFontManager::GetFont((eFont) (m_font+1))->Write(x, y, 0.0f, m_text, 0, m_alignment, (m_colors[1][0] & 0x00FFFFFF) | alpha, (m_colors[1][1] & 0x00FFFFFF) | alpha);
	
	
	int yoffset = 200;
	int y = yoffset;
	
	RudeFontManager::GetFont(kBigFontOutline)->Write(320/2, y, 0.0f, "Credits", 0, FONT_ALIGN_CENTER, 0xFF000000, 0xFF000000);
	RudeFontManager::GetFont(kBigFont)->Write(320/2, y, 0.0f, "Credits", 0, FONT_ALIGN_CENTER, 0xFFFFFFFF, 0xFFFFE9CA);
	
	
	
	typedef struct {
		const char *title;
		const char *name;
	} tName;
	
	const int kNumLines = 4;
	
	const tName kLines[kNumLines] = {
		{ "Programming:" , "Robert Rose" },
		{ "Art:" , "Jake Helms" },
		{ "Animation:" , "Keo Heng" },
		{ "Music:" , "Mick Rippon" }
	};
	
	y += 40;
	int inset = 50;
	
	for(int i = 0; i < kNumLines; i++)
	{
	
		RudeFontManager::GetFont(kDefaultFontOutline)->Write(inset, y, 0.0f, kLines[i].title, 0, FONT_ALIGN_LEFT, 0xFF000000, 0xFF000000);
		RudeFontManager::GetFont(kDefaultFont)->Write(inset, y, 0.0f, kLines[i].title, 0, FONT_ALIGN_LEFT, 0xFFFFFFFF, 0xFFCCCCCC);
		
		RudeFontManager::GetFont(kDefaultFontOutline)->Write(320 - inset, y, 0.0f, kLines[i].name, 0, FONT_ALIGN_RIGHT, 0xFF000000, 0xFF000000);
		RudeFontManager::GetFont(kDefaultFont)->Write(320 - inset, y, 0.0f, kLines[i].name, 0, FONT_ALIGN_RIGHT, 0xFFFFFFFF, 0xFFCCCCCC);
	
		y += 20;
	}
	
	const int kNumThanks = 4;
	
	const char * kThanks[kNumThanks] = {
		"Special thanks to Maria and Monica",
		"for making this game possible.",
		"",
		"www.bork3d.com"
	};
	
	y += 20;
	
	for(int i = 0; i < kNumThanks; i++)
	{
		RudeFontManager::GetFont(kDefaultFontOutline)->Write(320/2, y, 0.0f, kThanks[i], 0, FONT_ALIGN_CENTER, 0xFF000000, 0xFF000000);
		RudeFontManager::GetFont(kDefaultFont)->Write(320/2, y, 0.0f, kThanks[i], 0, FONT_ALIGN_CENTER, 0xFFFFFFFF, 0xFFCCCCCC);
		
		y+= 16;
	}
}	
