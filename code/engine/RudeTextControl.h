/*
 *  RudeTextControl.h
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef __H_RudeTextControl
#define __H_RudeTextControl

#include "RudeControl.h"
#include "RudeFont.h"

const int kMaxTextLen = 64;
const int kNumColors = 2;

typedef enum {
	kNoValue,
	kFloatValue,
	kIntValue,
	kSignedIntValue
} eRudeTextControlDisplayValue;

typedef enum {
	kNoStyle,
	kOutlineStyle,
} eRudeTextControlStyle;

class RudeTextControl : public RudeControl {
public:

	
	typedef enum {
		kAlignLeft,
		kAlignCenter,
		kAlignRight,
		kAlignJustify
	} eRudeTextControlAlignment;
	

	
	RudeTextControl(RudeControl *parent);
	
	bool Contains(const RudeScreenVertex &p);
	
	void Render();
	
	void SetPosition(int x, int y);
	void GetPosition(int &x, int &y) { x = m_x; y = m_y; }
	void SetStyle(eRudeTextControlStyle s) { m_style = s; }
	void SetAlignment(eRudeTextControlAlignment a) { m_alignment = a; }
	void SetFormat(eRudeTextControlDisplayValue value, const char *fmt);
	void SetValue(float f);
	void SetText(const char *text);
	void SetFont(eFont font) { m_font = font; }
	
	void SetColors(int i, unsigned int top, unsigned int bot)
	{
		RUDE_ASSERT(i >= 0 && i < kNumColors, "Invalid color index");

		m_colors[i][0] = top;
		m_colors[i][1] = bot;
	}
	
	void SetAlpha(float a)
	{ 
		if(a < 0.0f)
			a = 0.0f;
		else if(a > 1.0f)
			a = 1.0f;
		
		m_alpha = a; 
	}
	
protected:
	
	void Display(float x, float y);
	
	char m_text[kMaxTextLen];
	char m_format[kMaxTextLen];
	eRudeTextControlDisplayValue m_displayValue;
	eRudeTextControlAlignment m_alignment;
	eRudeTextControlStyle m_style;
	eFont m_font;
	
	unsigned int m_colors[kNumColors][2];
	
	float m_alpha;
	float m_value;

	int m_x;
	int m_y;
};

#endif
