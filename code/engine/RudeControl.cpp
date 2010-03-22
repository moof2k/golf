/*
 *  RudeControl.cpp
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#include "RudeControl.h"

#include "RudeDebug.h"
#include "RudeFile.h"

#include "RudeTextControl.h"
#include "RudeButtonControl.h"
#include "RudeButtonAnimControl.h"

#include <list>

RudeControl::RudeControl()
: m_rect(0,0,0,0)
, m_hitStart(0,0)
, m_hitMove(0,0)
, m_hitMoveDelta(0,0)
, m_hitDelta(0,0)
, m_hitDistanceTraveled(0,0)
, m_hitId(-1)
, m_translation(0,0,0)
, m_desiredTranslation(0,0,0)
, m_animSpeed(3.0f)
, m_animType(kAnimNone)
{
}

RudeControl::~RudeControl()
{
	unsigned int size = m_children.size();

	for(unsigned int i = 0; i < size; i++)
	{
		RUDE_ASSERT(m_children[i], "Invalid child discovered");

		delete m_children[i];
		m_children[i] = 0;
	}

	m_children.clear();
}

void RudeControl::Load(const char *name)
{
	char filename[256];
	snprintf(filename, 256, "%s.ui", name);

	char buffer[512];
	RudeFileGetFile(filename, buffer, 512, false);

	FILE *file = fopen(buffer, "r");
	RUDE_ASSERT(file, "Could not open %s", buffer);

	char line[512];
	while(fgets(line, 512, file) != 0)
	{
		RUDE_ASSERT(line, "Failed to parse %s\n", buffer);
		
		if(line[0] == '\n')
			continue;
		if(line[0] == '#')
			continue;

		ConstructChild(line);
	}

}

/**
 * Returns the RudeControl matching the given name.
 * If the control is not found this method asserts.
 * Make sure you dynamic_cast the control to the type
 * you expect.
 */
RudeControl * RudeControl::GetChildControl(const std::string &name)
{
	unsigned int size = m_children.size();

	for(unsigned int i = 0; i < size; i++)
	{
		RudeControl *child = m_children[i];
		RUDE_ASSERT(child, "Invalid child discovered");

		if(child->GetName() == name)
			return child;
	}

	RUDE_ASSERT(0, "Child not found '%s'", name.c_str());

	return 0;
}


RudeTextControl * RudeControl::GetChildTextControl(const std::string &name)
{
	RudeTextControl *child = dynamic_cast<RudeTextControl *>(GetChildControl(name));

	RUDE_ASSERT(child, "Child control '%s' not of expected type RudeTextControl", name.c_str());

	return child;
}

RudeButtonControl * RudeControl::GetChildButtonControl(const std::string &name)
{
	RudeButtonControl *child = dynamic_cast<RudeButtonControl *>(GetChildControl(name));

	RUDE_ASSERT(child, "Child control '%s' not of expected type RudeButtonControl", name.c_str());

	return child;
}

RudeButtonAnimControl * RudeControl::GetChildButtonAnimControl(const std::string &name)
{
	RudeButtonAnimControl *child = dynamic_cast<RudeButtonAnimControl *>(GetChildControl(name));

	RUDE_ASSERT(child, "Child control '%s' not of expected type RudeButtonAnimControl", name.c_str());

	return child;
}


bool RudeControl::Contains(const RudeScreenVertex &p)
{
	return m_rect.Contains(p);
}

bool RudeControl::TouchDown(RudeTouch *t)
{
	if(!Contains(t->m_location))
		return false;
	
	m_hitId = t->m_touchId;
	m_hitStart = t->m_location;
	m_hitMove = m_hitStart;
	m_hitMoveDelta = RudeScreenVertex(0,0);
	m_hitDelta = RudeScreenVertex(0,0);
	m_hitDistanceTraveled = RudeScreenVertex(0,0);
	
	return true;
}

bool RudeControl::TouchMove(RudeTouch *t)
{
	if(m_hitId != t->m_touchId)
		return false;
	
	m_hitMoveDelta = t->m_location - m_hitMove;
	m_hitMove = t->m_location;
	m_hitDelta = t->m_location - m_hitStart;
	
	m_hitDistanceTraveled.m_x += abs(m_hitDelta.m_x);
	m_hitDistanceTraveled.m_y += abs(m_hitDelta.m_y);
	
	return true;
}

bool RudeControl::TouchUp(RudeTouch *t)
{
	if(m_hitId != t->m_touchId) 
		return false;
	
	m_hitId = -1;
	
	m_hitMoveDelta = t->m_location - m_hitMove;
	m_hitMove = t->m_location;
	m_hitDelta = t->m_location - m_hitStart;
	
	return true;
}

void RudeControl::NextFrame(float delta)
{
	switch(m_animType)
	{
		default:
		case kAnimNone:
			break;
		case kAnimPopSlide:
			{
				m_translation += (m_desiredTranslation - m_translation) * delta * m_animSpeed;
			}
			break;
	}

	unsigned int size = m_children.size();
	for(unsigned int i = 0; i < size; i++)
		m_children[i]->NextFrame(delta);
}

void RudeControl::Render()
{
	unsigned int size = m_children.size();
	for(unsigned int i = 0; i < size; i++)
		m_children[i]->Render();

	RGL.LoadIdentity();
	
	if(m_animType != kAnimNone)
		RGL.Translate(m_translation.x(), m_translation.y(), m_translation.z());
}



void RudeControl::ConstructChild(char *desc)
{
	RUDE_ASSERT(desc, "Invalid description");

	// Store description in case we need to print it
	std::string originalDesc(desc);
	
	// Split description string into tokens
	std::list<std::string> tokens;

	const char *kDelimStr = "\t\n\r";
	const char *token = strtok(desc, kDelimStr);

	while(token)
	{
		tokens.push_back(std::string(token));
		token = strtok(0, kDelimStr);
	}

	// First token designates the type of Control
	std::string type = PopToken(tokens, originalDesc, "type");

	// Second token designates the name of the Control
	std::string name = PopToken(tokens, originalDesc, "name");

	// Construct the control based on it's type
	RudeControl *control = 0;

	if(type == "Text")
	{
		RudeTextControl *c = new RudeTextControl();
		RUDE_ASSERT(c, "Failed to construct control");

		// Text (content) of RudeTextControl
		std::string text = PopToken(tokens, originalDesc, "text");
		c->SetText(text.c_str());

		// Alignment
		std::string alignment = PopToken(tokens, originalDesc, "alignment");

		bool loadposition = true;

		if(alignment == "center")
		{
			c->SetAlignment(RudeTextControl::kAlignCenter);
			loadposition = false;

			// Rect {t,l,b,r}
			std::string rectstr = PopToken(tokens, originalDesc, "rect");

			RudeRect rect;
			ParseRect(rectstr, rect);
			c->SetRect(rect);
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
			std::string offset = PopToken(tokens, originalDesc, "offset");
			int offx = 0, offy = 0;
			ParseOffset(offset, offx, offy);

			c->SetPosition(offx, offy);
		}

		// Font
		std::string font = PopToken(tokens, originalDesc, "font");

		if(font == "default")
			c->SetFont(kDefaultFont);
		else if(font == "big")
			c->SetFont(kBigFont);
		else
		{
			RUDE_ASSERT(0, "Expected font field (default, big), got %s", font.c_str());
		}

		// Style
		std::string style = PopToken(tokens, originalDesc, "style");

		if(style == "none")
			c->SetStyle(kNoStyle);
		else if(style == "outline")
			c->SetStyle(kOutlineStyle);
		else
		{
			RUDE_ASSERT(0, "Expected style field (none, outline), got %s", style.c_str());
		}

		// Animation
		std::string anim = PopToken(tokens, originalDesc, "animation");

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
			ParseColor(colortopstr, colortop);

			unsigned int colorbottom = 0;
			ParseColor(colorbottomstr, colorbottom);

			c->SetColors(colorindex, colortop, colorbottom);

			colorindex++;
		}

		RUDE_ASSERT(tokens.size() == 0, "TextControl string contains extraneous tokens %s", originalDesc.c_str());

		control = c;
	}
	else if(type == "Button")
	{
		RudeButtonControl *c = new RudeButtonControl();
		RUDE_ASSERT(c, "Failed to construct control");

		// Texture
		std::string texture = PopToken(tokens, originalDesc, "texture");

		// Texture Offset
		std::string textureoffsets = PopToken(tokens, originalDesc, "texture offsets");
		int offx = 0, offy = 0;
		ParseOffset(textureoffsets, offx, offy);

		c->SetTexture(texture.c_str(), offx, offy);

		// Rect {t,l,b,r}
		std::string rectstr = PopToken(tokens, originalDesc, "rect");

		RudeRect rect;
		ParseRect(rectstr, rect);
		c->SetRect(rect);

		// Position Animation
		std::string anim = PopToken(tokens, originalDesc, "animation");

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

		control = c;
	}
	else if(type == "ButtonAnim")
	{
		RudeButtonAnimControl *c = new RudeButtonAnimControl();
		RUDE_ASSERT(c, "Failed to construct control");

		// Texture
		std::string texture = PopToken(tokens, originalDesc, "texture");

		// Texture Offset
		std::string textureoffsets = PopToken(tokens, originalDesc, "texture offsets");
		int offx = 0, offy = 0;
		ParseOffset(textureoffsets, offx, offy);

		c->SetTexture(texture.c_str(), offx, offy);

		// Rect {t,l,b,r}
		std::string rectstr = PopToken(tokens, originalDesc, "rect");

		RudeRect rect;
		ParseRect(rectstr, rect);
		c->SetRect(rect);

		// Position Animation
		std::string anim = PopToken(tokens, originalDesc, "animation");

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

		control = c;
	}
	
	RUDE_ASSERT(control, "Failed to create Control type: %s", type.c_str());

	control->SetName(name);

	// Add control to list of children
	m_children.push_back(control);

}

std::string RudeControl::PopToken(std::list<std::string> &tokens, std::string &desc, const std::string &explanation)
{
	RUDE_ASSERT(tokens.size() > 0, "Invalid description: '%s', Expected %s", desc.c_str(), explanation.c_str());
	std::string token = tokens.front();
	tokens.pop_front();

	return token;
}

void RudeControl::ParseRect(std::string &str, RudeRect &rect)
{
	char rectstr[256];
	snprintf(rectstr, 256, "%s", str.c_str());

	// Split rect string into tokens
	std::list<std::string> tokens;

	const char *kDelimStr = "{, }";
	const char *token = strtok(rectstr, kDelimStr);

	while(token)
	{
		tokens.push_back(std::string(token));
		token = strtok(0, kDelimStr);
	}

	std::string top = PopToken(tokens, str, "top (t,l,b,r)");
	std::string left = PopToken(tokens, str, "left (t,l,b,r)");
	std::string bottom = PopToken(tokens, str, "bottom (t,l,b,r)");
	std::string right = PopToken(tokens, str, "right (t,l,b,r)");

	rect.m_top = atoi(top.c_str());
	rect.m_left = atoi(left.c_str());
	rect.m_bottom = atoi(bottom.c_str());
	rect.m_right = atoi(right.c_str());
}

void RudeControl::ParseOffset(std::string &str, int &offx, int &offy)
{
	char offsetstr[256];
	snprintf(offsetstr, 256, "%s", str.c_str());

	// Split rect string into tokens
	std::list<std::string> tokens;

	const char *kDelimStr = "{, }";
	const char *token = strtok(offsetstr, kDelimStr);

	while(token)
	{
		tokens.push_back(std::string(token));
		token = strtok(0, kDelimStr);
	}

	std::string offxstr = PopToken(tokens, str, "x (x,y)");
	std::string offystr = PopToken(tokens, str, "y (x,y)");

	offx = atoi(offxstr.c_str());
	offy = atoi(offystr.c_str());
}

void RudeControl::ParseColor(std::string &str, unsigned int &color)
{
	int result = sscanf(str.c_str(), "0x%x", &color);

	RUDE_ASSERT(result == 1, "Failed to parse color (0xAARRGGBB), got %s\n", str.c_str());
}

