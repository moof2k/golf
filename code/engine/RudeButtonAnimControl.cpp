/*
 *  RudeButtonAnimControl.cpp
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#include "RudeButtonAnimControl.h"

#include "RudeGL.h"
#include "RudeTextureManager.h"




RudeButtonAnimControl::RudeButtonAnimControl(RudeControl *parent)
: RudeButtonControl(parent)
, m_data(0)
, m_numFrames(0)
, m_curFrame(0)
{
}


bool RudeButtonAnimControl::TouchDown(RudeTouch *t)
{
	if(!RudeControl::TouchDown(t))
		return false;
	
	
	return true;
}

bool RudeButtonAnimControl::TouchMove(RudeTouch *t)
{
	if(!RudeControl::TouchMove(t))
		return false;
	
	return true;
}

bool RudeButtonAnimControl::TouchUp(RudeTouch *t)
{
	if(!RudeControl::TouchUp(t))
		return false;
	
	return true;
}


void RudeButtonAnimControl::SetAnimData(const tRudeButtonAnimKeyframe *data, int numFrames)
{
	RUDE_ASSERT(data, "Invalid animation data");
	RUDE_ASSERT(numFrames > 0, "Invalid animation data");

	m_data.clear();
	
	for(int i = 0; i < numFrames; i++)
	{
		RUDE_ASSERT(data[i].m_time > 0.0f && data[i].m_time < 100.0f, "Unreasoable animation time");
		RUDE_ASSERT(data[i].m_texture >= 0, "Unreasonable animation texture");

		m_data.push_back(data[i]);
	}
	
	m_curFrame = 0;
	m_numFrames = numFrames;
}
									

void RudeButtonAnimControl::NextFrame(float delta)
{
	m_time += delta;
	
	while(m_time >= m_data[m_curFrame].m_time)
	{
		m_time -= m_data[m_curFrame].m_time;
		
		m_curFrame++;
		if(m_curFrame >= m_numFrames)
			m_curFrame = 0;
	}
}

void RudeButtonAnimControl::Render()
{
	RudeControl::Render();
	
	tRudeButtonAnimKeyframe &frame = m_data[m_curFrame];
	
	RudeTextureManager::GetInstance()->SetTexture(frame.m_texture);
	
	const unsigned int colors[] = {
		0xFFFFFFFF,
		0xFFFFFFFF,
		0xFFFFFFFF,
		0xFFFFFFFF
	};
	
	RGL.EnableClient(kVertexArray, true);
	RGL.EnableClient(kColorArray, true);
	RGL.EnableClient(kTextureCoordArray, true);
	
	glVertexPointer(2, GL_FLOAT, 0, m_points);
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, colors);
	glTexCoordPointer(2, GL_FLOAT, 0, m_uvs);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

/**
 * RudeButtonAnimControl factory assistant for RudeControl.  This is called by RudeControl::Load()
 */
RudeControl * ConstructButtonAnimControl(RudeControl *parent, std::list<std::string> &tokens, const std::string &originalDesc)
{
	RudeButtonAnimControl *c = new RudeButtonAnimControl(parent);
	RUDE_ASSERT(c, "Failed to construct control");

	// Texture
	std::string texture = RudeControl::PopToken(tokens, originalDesc, "texture");

	// Texture Offset
	std::string textureoffsets = RudeControl::PopToken(tokens, originalDesc, "texture offsets");
	int offx = 0, offy = 0;
	RudeControl::ParseOffset(textureoffsets, offx, offy);

	c->SetTexture(texture.c_str(), offx, offy);

	// Rect {t,l,b,r}
	std::string rectstr = RudeControl::PopToken(tokens, originalDesc, "rect");

	RudeRect rect;
	RudeControl::ParseRect(rectstr, rect);
	c->SetFileRect(rect);

	// Position Animation
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

	return c;
}

RudeControlRegistration buttonAnimRegistration("RudeButtonAnimControl", ConstructButtonAnimControl);

