/*
 *  RudeTextControl.cpp
 *  golf
 *
 *  Created by Robert Rose on 10/17/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "RudeTextControl.h"
#include "RudeFont.h"
#include "Rude.h"

RudeTextControl::RudeTextControl()
: m_displayValue(kNoValue)
, m_value(-123123.0f)
, m_alignment(kAlignLeft)
, m_style(kNoStyle)
, m_font(kDefaultFont)
{
	m_text[0] = '\0';
	m_format[0] = '\0';
	
	for(int i = 0; i < 2; i++)
	{
		m_colors[i][0] = 0xFFFFFFFF;
		m_colors[i][1] = 0xFFFFFFFF;
	}
}

// RudeFontManager::GetFont(kDefaultFont)->Printf(310.0f, kParY, 0.0f, FONT_ALIGN_RIGHT, kParTopColor, kParBotColor, "Stroke %d", m_stroke);

void RudeTextControl::SetPosition(int x, int y)
{
	const int kHeight = 8;
	
	switch(m_alignment)
	{
		case kAlignLeft:
			SetRect(RudeRect(y - kHeight, x, y + kHeight, x + 1));
			break;
		case kAlignRight:
			SetRect(RudeRect(y - kHeight, x-1, y + kHeight, x));
			break;
		case kAlignCenter:
			
			break;
	}
}

void RudeTextControl::Render()
{
	float y = (m_rect.m_bottom - m_rect.m_top) / 2 + m_rect.m_top;
	
	switch(m_alignment)
	{
		case kAlignLeft:
			Display(m_rect.m_left, y);
			break;
		case kAlignCenter:
			float x = (m_rect.m_right - m_rect.m_left) / 2 + m_rect.m_left;
			Display(x, y);
			break;
		case kAlignRight:
			Display(m_rect.m_right, y);
			break;
	}
}

void RudeTextControl::Display(float x, float y)
{
	if(m_style == kOutlineStyle)
		RudeFontManager::GetFont((eFont) (m_font+1))->Write(x, y, 0.0f, m_text, 0, m_alignment, m_colors[1][0], m_colors[1][1]);
	
	RudeFontManager::GetFont(m_font)->Write(x, y, 0.0f, m_text, 0, m_alignment, m_colors[0][0], m_colors[0][1]);

}

void RudeTextControl::SetFormat(eRudeTextControlDisplayValue value, const char *fmt)
{
	m_displayValue = value;
	
	RUDE_ASSERT(strlen(fmt) < kMaxTextLen, "Format text too long: %s", fmt);
	
	strcpy(m_format, fmt);
	
}

void RudeTextControl::SetValue(float v)
{
	if(m_value == v)
		return;
	
	m_value = v;
	
	char str[kMaxTextLen];
	
	switch(m_displayValue)
	{
		case kNoValue:
			return;
			break;
		case kFloatValue:
			sprintf(str, m_format, m_value);
			break;
		case kIntValue:
			sprintf(str, m_format, (int) m_value);
			break;
		case kSignedIntValue:
			{
				char foo[32];
				if(m_value == 0.0f)
					sprintf(foo, "±0");
				else
					sprintf(foo, "%d", m_value);
				
				sprintf(str, m_format, foo);
			}
			break;
	}
	
	SetText(str);
}

void RudeTextControl::SetText(const char *text)
{
	RUDE_ASSERT(strlen(text) < kMaxTextLen, "Text too long: %s", text);
	
	strcpy(m_text, text);
	
}



