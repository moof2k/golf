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

RBTerrainSliceControl::RBTerrainSliceControl(RudeControl *parent)
: RudeControl(parent)
, m_guideIndicatorButton(0)
, m_holeIndicatorButton(0)
, m_guide(0)
, m_holeDistance(0.0f)
, m_guideDistance(0.0f)
{
	m_guideIndicatorButton.SetTexture("guidem");
	m_holeIndicatorButton.SetTexture("guide2m");
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
	
	btVector3 holePosition(-1.0, 0.0, 0.0);
	btVector3 guidePosition(-1.0, 0.0, 0.0);

	float maxheight = m_guide->GetMaxGuidePoint();
	float minheight = m_guide->GetMinGuidePoint();

	float heightdiff = (maxheight - minheight);
	if(heightdiff < 1.0f)
		heightdiff = 1.0f;

	float scale = (m_drawRect.m_bottom - m_drawRect.m_top - 6) / heightdiff;

	float segmentWidth = (m_drawRect.m_right - m_drawRect.m_left) / float(numGuidePoints-1);

	for(int i = 1; i < numGuidePoints; i++)
	{
		float h0 = guidePoints[i-1].m_height - minheight;
		float h1 = guidePoints[i].m_height - minheight;

		RudeColorFloat &c0 = guidePoints[i-1].m_color;
		RudeColorFloat &c1 = guidePoints[i].m_color;
		
		float d0 = guidePoints[i-1].m_distance;
		float d1 = guidePoints[i].m_distance;

		//const float kPointSize = 0.2;

		float left = m_drawRect.m_right - i * segmentWidth;
		float right = m_drawRect.m_right - (i-1) * segmentWidth;
		float p0 = m_drawRect.m_bottom - h0 * scale;
		float p1 = m_drawRect.m_bottom - h1 * scale;

		GLfloat point[] = {
			left, p1,
			right, p0,
			right, m_drawRect.m_bottom,
			left, m_drawRect.m_bottom
		};

		const float kBgAlpha = 0.3f;

		GLfloat bgcolors[] = {
			c1.m_r, c1.m_g, c1.m_b, kBgAlpha,
			c0.m_r, c0.m_g, c0.m_b, kBgAlpha,
			c0.m_r, c0.m_g, c0.m_b, kBgAlpha,
			c1.m_r, c1.m_g, c1.m_b, kBgAlpha,	
		};


		glVertexPointer(2, GL_FLOAT, 0, point);
		glColorPointer(4, GL_FLOAT, 0, bgcolors);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		const float kLineAlpha = 0.7f;

		GLfloat linecolors[] = {
			c1.m_r, c1.m_g, c1.m_b, kLineAlpha,
			c0.m_r, c0.m_g, c0.m_b, kLineAlpha,
		};

		glColorPointer(4, GL_FLOAT, 0, linecolors);
		glDrawArrays(GL_LINES, 0, 2);
	
		// Render hash marks below
		if(i % 10 == 0)
		{

			GLfloat point[] = {
				m_drawRect.m_right - i * segmentWidth, m_drawRect.m_bottom,
				m_drawRect.m_right - i * segmentWidth, m_drawRect.m_bottom + 3,
			};

			glVertexPointer(2, GL_FLOAT, 0, point);
			glColorPointer(4, GL_FLOAT, 0, linecolors);
			glDrawArrays(GL_LINES, 0, 2);
		}
		
		// Determine offset of hole marker
		if(m_holeDistance <= d0 && m_holeDistance >= d1)
		{
			holePosition.setX(right);
			holePosition.setY(p0);
		}
		
		// Determine offset of guide marker
		if(m_guideDistance <= d0 && m_guideDistance >= d1)
		{
			guidePosition.setX(right);
			guidePosition.setY(p0);
		}
	}
	
	const int kGuideSize = 32;
	
	if(holePosition.x() >= 0.0f)
	{
		RudeRect holeRect(
						  (int) holePosition.y() - kGuideSize,
						  (int) holePosition.x() - kGuideSize,
						  (int) holePosition.y() + kGuideSize,
						  (int) holePosition.x() + kGuideSize
						  );
		m_holeIndicatorButton.SetDrawRect(holeRect);
		
		m_holeIndicatorButton.Render();
	}
	
	if(guidePosition.x() >= 0.0f)
	{
		RudeRect guideRect(
						  (int) guidePosition.y() - kGuideSize,
						  (int) guidePosition.x() - kGuideSize,
						  (int) guidePosition.y() + kGuideSize,
						  (int) guidePosition.x() + kGuideSize
						  );
		m_guideIndicatorButton.SetDrawRect(guideRect);
		
		m_guideIndicatorButton.Render();
		
	}
}

void RBTerrainSliceControl::SetCoursePositions(const btVector3 &ball, const btVector3 &hole, const btVector3 &guide)
{
	btVector3 ballToHole = ball - hole;
	m_holeDistance = ballToHole.length();
	
	btVector3 ballToGuide = ball - guide;
	m_guideDistance = ballToGuide.length();
}

/**
 * RBTerrainSlideControl factory assistant for RudeControl.  This is called by RudeControl::Load()
 */
RudeControl * ConstructRBTerrainSliceControl(RudeControl *parent, std::list<std::string> &tokens, const std::string &originalDesc)
{
	RBTerrainSliceControl *c = new RBTerrainSliceControl(parent);
	RUDE_ASSERT(c, "Failed to construct control");

	// Rect {t,l,b,r}
	std::string rectstr = RudeControl::PopToken(tokens, originalDesc, "rect");

	RudeRect rect;
	RudeControl::ParseRect(rectstr, rect);
	c->SetFileRect(rect);

	return c;
}

RudeControlRegistration rbTerrainSliceControl("RBTerrainSliceControl", ConstructRBTerrainSliceControl);



