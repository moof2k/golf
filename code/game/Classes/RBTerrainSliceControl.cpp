/*
 *  RBTerrainSliceControl.cpp
 *  golf
 *
 *  Created by Robert Rose on 10/30/08.
 *  Copyright 2008 Bork 3D LLC. All rights reserved.
 *
 */

#include "RBTerrainSliceControl.h"
#include "RudeGL.h"
#include "RudeDebug.h"

RBTerrainSliceControl::RBTerrainSliceControl()
{
	
}



void RBTerrainSliceControl::NextFrame(float delta)
{
	
}

void RBTerrainSliceControl::Render()
{

	glDisable(GL_TEXTURE_2D);

	RGL.EnableClient(kVertexArray, true);
	RGL.EnableClient(kColorArray, true);
	RGL.EnableClient(kTextureCoordArray, false);

	int numGuidePoints = m_guide->GetNumGuidePoints();
	RBBallGuidePoint *guidePoints = m_guide->GetGuidePoints();

	if(numGuidePoints < 2)
		return;

	float maxheight = m_guide->GetMaxGuidePoint();
	float minheight = m_guide->GetMinGuidePoint();

	float scale = (m_rect.m_bottom - m_rect.m_top) / (maxheight - minheight);
	float center = (m_rect.m_bottom - m_rect.m_top) / 2.0f;

	float segmentWidth = (m_rect.m_right - m_rect.m_left) / float(numGuidePoints-1);

	for(int i = 1; i < numGuidePoints; i++)
	{
		float h0 = guidePoints[i-1].m_height - minheight;
		float h1 = guidePoints[i].m_height - minheight;

		RudeColorFloat &c0 = guidePoints[i-1].m_color;
		RudeColorFloat &c1 = guidePoints[i].m_color;

		//const float kPointSize = 0.2;

		GLfloat point[] = {
			m_rect.m_right - (i-1) * segmentWidth, m_rect.m_bottom - h0 * scale,
			m_rect.m_right - i * segmentWidth, m_rect.m_bottom - h1 * scale,
		};

		float a = 0.7f;



		GLfloat colors[] = {
			c0.m_r, c0.m_g, c0.m_b, a,
			c1.m_r, c1.m_g, c1.m_b, a,
		};


		glVertexPointer(2, GL_FLOAT, 0, point);
		glColorPointer(4, GL_FLOAT, 0, colors);
		glDrawArrays(GL_LINES, 0, 4);

		if(i % 10 == 0)
		{
			GLfloat point[] = {
				m_rect.m_right - i * segmentWidth, m_rect.m_bottom,
				m_rect.m_right - i * segmentWidth, m_rect.m_bottom + 3,
			};

			glVertexPointer(2, GL_FLOAT, 0, point);
			glColorPointer(4, GL_FLOAT, 0, colors);
			glDrawArrays(GL_LINES, 0, 4);
		}
	}
}



/**
 * RBTerrainSlideControl factory assistant for RudeControl.  This is called by RudeControl::Load()
 */
RudeControl * ConstructRBTerrainSliceControl(std::list<std::string> &tokens, const std::string &originalDesc)
{
	RBTerrainSliceControl *c = new RBTerrainSliceControl();
	RUDE_ASSERT(c, "Failed to construct control");

	// Rect {t,l,b,r}
	std::string rectstr = RudeControl::PopToken(tokens, originalDesc, "rect");

	RudeRect rect;
	RudeControl::ParseRect(rectstr, rect);
	c->SetRect(rect);

	return c;
}

RudeControlRegistration rbTerrainSliceControl("RBTerrainSliceControl", ConstructRBTerrainSliceControl);



