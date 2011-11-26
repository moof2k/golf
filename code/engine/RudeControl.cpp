/*
 *  RudeControl.cpp
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009-2011 Bork 3D LLC. All rights reserved.
 *
 *	Permission is granted to use this software, in source code or binary form,
 *	with or without modification, for NONCOMMERCIAL PURPOSES provided:
 *	
 *	1) The user of this software (YOU) does not do so in a means intended to
 *	derive monetary compensation or commercial advantage.
 *	
 *	2) Redistributions of the source code contain this license notice, unmodified.
 *	
 *	3) Redistributions in binary form give credit to this software using the
 *	text, "Made with the Bork 3D Game Engine," either in an "About," "Credits,"
 *	or other prominent location in the binary run-time form.
 *	
 *	Commercial users of this software must obtain a Bork 3D Game Engine Commercial
 *	Use License.  See http://bork3d.com for details.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *	THE SOFTWARE.
 */

#include "RudeControl.h"

#include "RudeDebug.h"
#include "RudeFile.h"

#include "RudeTextControl.h"
#include "RudeButtonControl.h"
#include "RudeButtonAnimControl.h"


RudeControl::RudeControl(RudeControl *parent)
: m_parent(parent)
, m_fileRect(0,0,0,0)
, m_drawRect(0,0,0,0)
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
, m_adjustDrawRectToEdges(true)
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
		
		int len = strlen(line);

		if(len <= 0)
			continue;

		if(line[0] == '\n')
			// Blank line
			continue;
		if(line[0] == '#')
			// Comment
			continue;

		if(line[0] == char(0xEF))
			// UTF-8 escape sequence
			continue;

		ConstructChild(line);
	}

	SetName(std::string(name));

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

void RudeControl::SetFileRect(const RudeRect &fileRect)
{
	m_fileRect = fileRect;

	UpdateDrawRect();
}

void RudeControl::SetDrawRect(const RudeRect &drawRect)
{
	m_drawRect = drawRect;
	OnReposition();
}

void RudeControl::UpdateDrawRect()
{
	// Re-compute screen rect given file rect
	RudeRect drawRect = m_fileRect;

	// Fetch parent's rect, if no parent, set it to the screen
	RudeRect parentRect(0, 0, (int) RGL.GetDeviceHeight(), (int) RGL.GetDeviceWidth());

	if(m_parent)
		parentRect = m_parent->GetDrawRect();

	if(drawRect.m_top >= 0 || !m_adjustDrawRectToEdges)
		drawRect.m_top += parentRect.m_top;
	else
		drawRect.m_top += parentRect.m_bottom + 1;

	if(drawRect.m_left >= 0 || !m_adjustDrawRectToEdges)
		drawRect.m_left += parentRect.m_left;
	else
		drawRect.m_left += parentRect.m_right + 1;

	if(drawRect.m_bottom >= 0 || !m_adjustDrawRectToEdges)
		drawRect.m_bottom += parentRect.m_top;
	else
		drawRect.m_bottom += parentRect.m_bottom + 1;

	if(drawRect.m_right >= 0 || !m_adjustDrawRectToEdges)
		drawRect.m_right += parentRect.m_left;
	else
		drawRect.m_right += parentRect.m_right + 1;

	SetDrawRect(drawRect);

	// Notify children
	unsigned int size = m_children.size();
	for(unsigned int i = 0; i < size; i++)
		m_children[i]->UpdateDrawRect();
}

bool RudeControl::Contains(const RudeScreenVertex &p)
{
	return m_drawRect.Contains(p);
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

	ConstructRudeControlFuncPtr funcptr = RudeControlRegistration::GetConstructor(type);
	
	control = (*funcptr)(this, tokens, originalDesc);
	
	RUDE_ASSERT(control, "Failed to create Control type: %s", type.c_str());

	control->SetName(name);

	// Add control to list of children
	m_children.push_back(control);

}

std::string RudeControl::PopToken(std::list<std::string> &tokens, const std::string &desc, const std::string &explanation)
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


/**
 * RudeControl factory assistant for RudeControl.  This is called by RudeControl::Load()
 * Sometimes all you need is a Control.
 */
RudeControl * ConstructControl(RudeControl *parent, std::list<std::string> &tokens, const std::string &originalDesc)
{
	RudeControl *c = new RudeControl(parent);
	RUDE_ASSERT(c, "Failed to construct control");

	// Rect {t,l,b,r}
	std::string rectstr = RudeControl::PopToken(tokens, originalDesc, "rect");

	RudeRect rect;
	RudeControl::ParseRect(rectstr, rect);
	c->SetFileRect(rect);

	return c;
}

RudeControlRegistration controlRegistration("RudeControl", ConstructControl);


