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



RudeTextControl::RudeTextControl(RudeControl *parent)
: RudeControl(parent)
, m_displayValue(kNoValue)
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
	RudeRect r(m_drawRect);
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
			SetFileRect(RudeRect(y - kHeight, x, y + kHeight, x + 1));
			break;
		case kAlignRight:
			SetFileRect(RudeRect(y - kHeight, x-1, y + kHeight, x));
			break;
		case RudeTextControl::kAlignCenter:
			break;
        default:
            break;
	}
}

void RudeTextControl::Render()
{
	RudeControl::Render();
	
	float y = (float) ((m_drawRect.m_bottom - m_drawRect.m_top) / 2 + m_drawRect.m_top);
	
	switch(m_alignment)
	{
		case RudeTextControl::kAlignLeft:
			Display((float) m_drawRect.m_left, y);
			break;
		case RudeTextControl::kAlignCenter:
			{
				float x = (float) ((m_drawRect.m_right - m_drawRect.m_left) / 2 + m_drawRect.m_left);
				Display(x, y);
			}
			break;
		case kAlignRight:
			Display((float) m_drawRect.m_right, y);
			break;
        default:
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


/**
 * RudeTextControl factory assistant for RudeControl.  This is called by RudeControl::Load()
 */
RudeControl * ConstructTextControl(RudeControl *parent, std::list<std::string> &tokens, const std::string &originalDesc)
{
	RudeTextControl *c = new RudeTextControl(parent);
	RUDE_ASSERT(c, "Failed to construct control");

	// Text (content) of RudeTextControl
	std::string text = RudeControl::PopToken(tokens, originalDesc, "text");
	c->SetText(text.c_str());

	// Alignment
	std::string alignment = RudeControl::PopToken(tokens, originalDesc, "alignment");

	bool loadposition = true;

	if(alignment == "center")
	{
		c->SetAlignment(RudeTextControl::kAlignCenter);
		loadposition = false;

		// Rect {t,l,b,r}
		std::string rectstr = RudeControl::PopToken(tokens, originalDesc, "rect");

		RudeRect rect;
		RudeControl::ParseRect(rectstr, rect);
		c->SetFileRect(rect);
	}
	else if(alignment == "left")
		c->SetAlignment(RudeTextControl::kAlignLeft);
	else if(alignment == "right")
		c->SetAlignment(RudeTextControl::kAlignRight);
	else if(alignment == "justify")
		c->SetAlignment(RudeTextControl::kAlignJustify);
	else
	{
		RUDE_ASSERT(0, "Expected alignment field (center, left, right, justify), got %s", alignment.c_str());
	}

	if(loadposition)
	{
		// Position
		std::string offset = RudeControl::PopToken(tokens, originalDesc, "offset");
		int offx = 0, offy = 0;
		RudeControl::ParseOffset(offset, offx, offy);

		c->SetPosition(offx, offy);
	}

	// Font
	std::string font = RudeControl::PopToken(tokens, originalDesc, "font");

	if(font == "default")
	c->SetFont(kDefaultFont);
	else if(font == "big")
	c->SetFont(kBigFont);
	else
	{
		RUDE_ASSERT(0, "Expected font field (default, big), got %s", font.c_str());
	}

	// Style
	std::string style = RudeControl::PopToken(tokens, originalDesc, "style");

	if(style == "none")
	c->SetStyle(kNoStyle);
	else if(style == "outline")
	c->SetStyle(kOutlineStyle);
	else
	{
		RUDE_ASSERT(0, "Expected style field (none, outline), got %s", style.c_str());
	}

	// Animation
	std::string anim = RudeControl::PopToken(tokens, originalDesc, "animation");

	if(anim == "none")
	c->SetAnimType(kAnimNone);
	else if(anim == "constant")
	c->SetAnimType(kAnimConstant);
	else if(anim == "popslide")
	c->SetAnimType(kAnimPopSlide);
	else
	{
		RUDE_ASSERT(0, "Expected anim field (none, constant, popslide), got %s", anim.c_str());
	}

	// Colors are optional and are parsed in this order:
	// color0-top color0-bottom
	// color1-top color1-bottom

	int colorindex = 0;
	while(tokens.size() >= 2)
	{
		std::string colortopstr = tokens.front();
		tokens.pop_front();

		std::string colorbottomstr = tokens.front();
		tokens.pop_front();

		unsigned int colortop = 0;
		RudeControl::ParseColor(colortopstr, colortop);

		unsigned int colorbottom = 0;
		RudeControl::ParseColor(colorbottomstr, colorbottom);

		c->SetColors(colorindex, colortop, colorbottom);

		colorindex++;
	}

	RUDE_ASSERT(tokens.size() == 0, "TextControl string contains extraneous tokens %s", originalDesc.c_str());

	return c;
}

RudeControlRegistration textRegistration("RudeTextControl", ConstructTextControl);


