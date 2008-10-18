/*
 *  RudeTextControl.h
 *  golf
 *
 *  Created by Robert Rose on 10/17/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __H_RudeTextControl
#define __H_RudeTextControl

#include "RudeControl.h"

const int kMaxTextLen = 64;

typedef enum {
	kNoValue,
	kFloatValue,
	kIntValue,
	kSignedIntValue
} eRudeTextControlDisplayValue;

typedef enum {
	kAlignLeft,
	kAlignCenter,
	kAlignRight,
	kAlignJustify
} eRudeTextControlAlignment;

typedef enum {
	kNoStyle,
	kOutlineStyle,
} eRudeTextControlStyle;

class RudeTextControl : public RudeControl {
public:
	
	RudeTextControl();
	
	void Render();
	
	void SetPosition(int x, int y);
	void SetStyle(eRudeTextControlStyle s) { m_style = s; }
	void SetAlignment(eRudeTextControlAlignment a) { m_alignment = a; }
	void SetFormat(eRudeTextControlDisplayValue value, const char *fmt);
	void SetValue(float f);
	void SetText(const char *text);
	
	void SetColors(int i, unsigned int top, unsigned int bot)
	{
		m_colors[i][0] = top;
		m_colors[i][1] = bot;
	}
	
protected:
	
	void Display(float x, float y);
	
	char m_text[kMaxTextLen];
	char m_format[kMaxTextLen];
	eRudeTextControlDisplayValue m_displayValue;
	eRudeTextControlAlignment m_alignment;
	eRudeTextControlStyle m_style;
	
	unsigned int m_colors[2][2];
	
	float m_value;
};

#endif
