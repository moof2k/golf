/*
 *  RudeTextControl.cpp
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#include "RudeTextControl.h"
#include "RudeFont.h"
#include "Rude.h"
#include "RudeGL.h"



RudeTextControl::RudeTextControl()
: m_displayValue(kNoValue)
, m_value(-123123.0f)
, m_alignment(RudeTextControl::kAlignLeft)
, m_style(kNoStyle)
, m_font(kDefaultFont)
, m_alpha(1.0f)
, m_x(0)
, m_y(0)
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

bool RudeTextControl::Contains(const RudeScreenVertex &p)
{
	RudeRect r(m_rect);
	r.m_top -= 5;
	r.m_bottom -= 5;
	
	return r.Contains(p);
}

void RudeTextControl::SetPosition(int x, int y)
{
	const int kHeight = 8;

	m_x = x;
	m_y = y;
	
	switch(m_alignment)
	{
		case RudeTextControl::kAlignLeft:
			SetRect(RudeRect(y - kHeight, x, y + kHeight, x + 1));
			break;
		case kAlignRight:
			SetRect(RudeRect(y - kHeight, x-1, y + kHeight, x));
			break;
		case RudeTextControl::kAlignCenter:
			
			break;
	}
}

void RudeTextControl::Render()
{
	RudeControl::Render();
	
	float y = (float) ((m_rect.m_bottom - m_rect.m_top) / 2 + m_rect.m_top);
	
	switch(m_alignment)
	{
		case RudeTextControl::kAlignLeft:
			Display((float) m_rect.m_left, y);
			break;
		case RudeTextControl::kAlignCenter:
			{
				float x = (float) ((m_rect.m_right - m_rect.m_left) / 2 + m_rect.m_left);
				Display(x, y);
			}
			break;
		case kAlignRight:
			Display((float) m_rect.m_right, y);
			break;
	}
}

void RudeTextControl::Display(float x, float y)
{
	unsigned int alpha = int(255.0f * m_alpha);
	alpha = alpha << 24;
	
	if(m_style == kOutlineStyle)
		RudeFontManager::GetFont((eFont) (m_font+1))->Write(x, y, 0.0f, m_text, 0, m_alignment, (m_colors[1][0] & 0x00FFFFFF) | alpha, (m_colors[1][1] & 0x00FFFFFF) | alpha);
	
	RudeFontManager::GetFont(m_font)->Write(x, y, 0.0f, m_text, 0, m_alignment, (m_colors[0][0] & 0x00FFFFFF) | alpha, (m_colors[0][1] & 0x00FFFFFF) | alpha);

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
				else if(m_value > 0.0f)
					sprintf(foo, "+%d", (int) m_value);
				else
					sprintf(foo, "%d", (int) m_value);
				
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



