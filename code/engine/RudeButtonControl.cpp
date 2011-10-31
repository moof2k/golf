/*
 *  RudeButtonControl.cpp
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#include "RudeButtonControl.h"

#include "RudeGL.h"
#include "RudeTextureManager.h"



RudeButtonControl::RudeButtonControl(RudeControl *parent)
: RudeControl(parent)
, m_state(false)
, m_texid(-1)
, m_texsize(64)
{
}

/**
 * Specifies the texture this control should use for rendering.  You may optionally supply
 * an offset that will be used to select where in the texture the control should render.
 * Using offsets allows you to share multiple textures between controls (a texture almanac).
 *
 * If you don't specify an offset (by supplying -1) the entire texture will be rendered 1:1
 * within the rectangle.
 *
 * This method should be called before SetRect().  SetRect() is what actually sets up
 * the position and UV coordinates used for drawing.
 *
 * @param texturename The name of the texture.
 * @param offx The X-offset of the top-left corner for rendering.
 * @param offy The Y-offset of the top-left corner for rendering.
 */
void RudeButtonControl::SetTexture(const char *texturename, int offx, int offy)
{
	m_texid = RudeTextureManager::GetInstance()->LoadTextureFromPNGFile(texturename);
	
	RudeTexture *tex = RudeTextureManager::GetInstance()->GetTexture(m_texid);
	RUDE_ASSERT(tex, "Could not locate texture");

	RUDE_ASSERT(tex->GetHeight() == tex->GetWidth(), "ButtonControl textures are assumed to have equal height and width");
	m_texsize = tex->GetHeight();

	m_offx = offx;
	m_offy = offy;

	if(m_offx == -1 || m_offy == -1)
	{
		RUDE_ASSERT(m_offx == -1 && m_offy == -1, "Both X and Y offsets must be specified or none at all");
	}
}


bool RudeButtonControl::TouchDown(RudeTouch *t)
{
	if(!RudeControl::TouchDown(t))
		return false;

	
	return true;
}

bool RudeButtonControl::TouchMove(RudeTouch *t)
{
	if(!RudeControl::TouchMove(t))
		return false;
	
	return true;
}

bool RudeButtonControl::TouchUp(RudeTouch *t)
{
	if(!RudeControl::TouchUp(t))
		return false;
	
	return true;
}

void RudeButtonControl::OnReposition()
{
	if(m_offx == -1)
	{
		// Render the entire texture 1:1, centered within the rectangle

		int halfText = m_texsize / 2;

		int xc = (m_drawRect.m_right - m_drawRect.m_left) / 2;
		int yc = (m_drawRect.m_bottom - m_drawRect.m_top) / 2;
		float left = ((float) xc - halfText + m_drawRect.m_left);
		float top = ((float) yc - halfText + m_drawRect.m_top);

		m_points[0] = left;
		m_points[1] = top + m_texsize;

		m_points[2] = left;
		m_points[3] = top;

		m_points[4] = left + m_texsize;
		m_points[5] = top;

		m_points[6] = left + m_texsize;
		m_points[7] = top + m_texsize;

		m_uvs[0] = 0.0f;
		m_uvs[1] = 1.0f;

		m_uvs[2] = 0.0f;
		m_uvs[3] = 0.0f;

		m_uvs[4] = 1.0f;
		m_uvs[5] = 0.0f;

		m_uvs[6] = 1.0f;
		m_uvs[7] = 1.0f;
	}
	else
	{
		// Render only the supplied portions of the texture, and use the whole rectangle

		m_points[0] = (float) m_drawRect.m_left;
		m_points[1] = (float) m_drawRect.m_bottom;

		m_points[2] = (float) m_drawRect.m_left;
		m_points[3] = (float) m_drawRect.m_top;

		m_points[4] = (float) m_drawRect.m_right;
		m_points[5] = (float) m_drawRect.m_top;

		m_points[6] = (float) m_drawRect.m_right;
		m_points[7] = (float) m_drawRect.m_bottom;

		float width = ((float) m_drawRect.m_right - m_drawRect.m_left) / ((float) m_texsize);
		float height = ((float) m_drawRect.m_bottom - m_drawRect.m_top) / ((float) m_texsize);

		float texoffx = ((float) m_offx) / ((float) m_texsize);
		float texoffy = ((float) m_offy) / ((float) m_texsize);

		m_uvs[0] = texoffx;
		m_uvs[1] = texoffy + height;

		m_uvs[2] = texoffx;
		m_uvs[3] = texoffy;

		m_uvs[4] = texoffx + width;
		m_uvs[5] = texoffy;

		m_uvs[6] = texoffx + width;
		m_uvs[7] = texoffy + height;
	}

}

void RudeButtonControl::Render()
{
	RudeControl::Render();
	
	RudeTextureManager::GetInstance()->SetTexture(m_texid);

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
 * RudeButtonControl factory assistant for RudeControl.  This is called by RudeControl::Load()
 */
RudeControl * ConstructButtonControl(RudeControl *parent, std::list<std::string> &tokens, const std::string &originalDesc)
{
	RudeButtonControl *c = new RudeButtonControl(parent);
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

RudeControlRegistration buttonRegistration("RudeButtonControl", ConstructButtonControl);


