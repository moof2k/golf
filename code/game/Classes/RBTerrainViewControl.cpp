/*
 *  RBTerrainViewControl.cpp
 *  golf
 *
 *  Created by Robert Rose on 10/30/08.
 *  Copyright 2008 Bork 3D LLC. All rights reserved.
 *
 */

#include "RBTerrainViewControl.h"
#include "RudeGL.h"
#include "RudeDebug.h"

RBTerrainViewControl::RBTerrainViewControl()
: m_terrain(0)
, m_originx(0.0f)
, m_originy(0.0f)
, m_angle(0.0f)
, m_scale(1.0f)
, m_bgTimer(0.0f)
{
	m_guideIndicatorButton.SetTexture("guide");
	m_ballIndicatorButton.SetTexture("balldot");
	m_holeIndicatorButton.SetTexture("guide2");
}


void RBTerrainViewControl::NextFrame(float delta)
{
	m_bgTimer += delta;
}

void RBTerrainViewControl::SetPositions(const btVector3 &ball, const btVector3 &hole, const btVector3 &guide)
{
	//RUDE_REPORT("ball %f %f %f\n", ball.x(), ball.y(), ball.z());
	//RUDE_REPORT("guide %f %f %f\n", guide.x(), guide.y(), guide.z());

	// Try to keep the ball and the hole within this box
	const float kDesiredBoxSize = 680.0f;

	btVector3 ballToHole = hole - ball;

	btVector3 center = ball + (ballToHole / 2.0f);

	m_originx = center.x();
	m_originy = center.z();

	float ballToHoleLen = ballToHole.length();

	m_scale = ballToHoleLen / kDesiredBoxSize;

	// Cap the scale at 2x
	if(m_scale > 2.0f)
		m_scale = 2.0f;
	if(m_scale < 0.1f)
		m_scale = 0.1f;

	m_guide = guide;
	m_ball = ball;
	m_hole = hole;

	m_bgTimer = 0.0f;
}

void RBTerrainViewControl::Translate(float x, float y)
{
	m_originx += x * m_scale;
	m_originy += y * m_scale;

	const float kMaxTranslation = 2000.0f;

	if(m_originx < -kMaxTranslation)
		m_originx = -kMaxTranslation;
	if(m_originx > kMaxTranslation)
		m_originx = kMaxTranslation;

	if(m_originy < -kMaxTranslation)
		m_originy = -kMaxTranslation;
	if(m_originy > kMaxTranslation)
		m_originy = kMaxTranslation;
}


void RBTerrainViewControl::Render()
{
	if(!m_terrain)
		return;

	RGL.Enable(kBackfaceCull, false);
	RGL.Enable(kDepthTest, false);
	
	int height = m_rect.m_bottom - m_rect.m_top;
	int width = m_rect.m_right - m_rect.m_left;

	RGL.SetViewport(m_rect.m_top, m_rect.m_left, m_rect.m_bottom, m_rect.m_right);
	RGL.Ortho(0.0f, 0.0f, 0.0f, (float) width, (float) height, 1000.0f);
	RGL.LoadIdentity();

	glDisable(GL_TEXTURE_2D);

	RGL.EnableClient(kVertexArray, true);
	RGL.EnableClient(kColorArray, true);
	RGL.EnableClient(kTextureCoordArray, false);

	float bgalpha = 4.0f * m_bgTimer * 0.5f;

	if(bgalpha > 0.5f)
		bgalpha = 0.5f;

	GLfloat point[] = {
		0, 0,
		0, height,
		width, height,
		width, 0
	};

	GLfloat colors[] = {
		0.0f, 0.0f, 0.0f, bgalpha,
		0.0f, 0.0f, 0.0f, bgalpha,
		0.0f, 0.0f, 0.0f, bgalpha,
		0.0f, 0.0f, 0.0f, bgalpha,
	};


	glVertexPointer(2, GL_FLOAT, 0, point);
	glColorPointer(4, GL_FLOAT, 0, colors);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glEnable(GL_TEXTURE_2D);

	float ox = m_originx - width/2 * m_scale;
	float oy = -m_originy + height/2 * m_scale;

	RGL.Ortho(ox, oy, -100.0f, width * m_scale, height * m_scale, 1000.0f);
	
	//RGL.ScaleView(1.0f / m_scale, 1.0f / m_scale, 1.0f / m_scale);
	//RGL.TranslateView(ox, oy, -100.0);
	

	//RGL.RotateView(-90.0, 1.0, 0.0, 0.0);
	//RGL.RotateView(m_angle, 0.0, 0.0, 1.0);
	
	RGL.Rotate(-90.0, 1.0, 0.0, 0.0);

	m_terrain->RenderInBoundsOnly();


	RGL.Ortho(0.0, 0.0, -100.0f, (float) width, (float) height, 1000.0f);

	ox = -m_originx / m_scale + width/2;
	oy = -m_originy / m_scale + height/2;


	const int kGuideSize = 32;
	RudeRect guideRect(
		(int) (m_guide.z() / m_scale + oy - kGuideSize),
		(int) (m_guide.x() / m_scale + ox - kGuideSize),
		(int) (m_guide.z() / m_scale + oy + kGuideSize),
		(int) (m_guide.x() / m_scale + ox + kGuideSize)
		);

	RudeRect ballRect(
		(int) (m_ball.z() / m_scale + oy - kGuideSize),
		(int) (m_ball.x() / m_scale + ox - kGuideSize),
		(int) (m_ball.z() / m_scale + oy + kGuideSize),
		(int) (m_ball.x() / m_scale + ox + kGuideSize)
		);

	RudeRect holeRect(
		(int) (m_hole.z() / m_scale + oy - kGuideSize),
		(int) (m_hole.x() / m_scale + ox - kGuideSize),
		(int) (m_hole.z() / m_scale + oy + kGuideSize),
		(int) (m_hole.x() / m_scale + ox + kGuideSize)
		);
	
	m_guideIndicatorButton.SetRect(guideRect);
	m_guideIndicatorButton.Render();

	m_holeIndicatorButton.SetRect(holeRect);
	m_holeIndicatorButton.Render();

	m_ballIndicatorButton.SetRect(ballRect);
	m_ballIndicatorButton.Render();


}



/**
 * ConstructRBTerrainViewControl factory assistant for RudeControl.  This is called by RudeControl::Load()
 */
RudeControl * ConstructRBTerrainViewControl(std::list<std::string> &tokens, const std::string &originalDesc)
{
	RBTerrainViewControl *c = new RBTerrainViewControl();
	RUDE_ASSERT(c, "Failed to construct control");

	// Rect {t,l,b,r}
	std::string rectstr = RudeControl::PopToken(tokens, originalDesc, "rect");

	RudeRect rect;
	RudeControl::ParseRect(rectstr, rect);
	c->SetRect(rect);

	return c;
}

RudeControlRegistration rbTerrainViewControlRegistration("RBTerrainViewControl", ConstructRBTerrainViewControl);



