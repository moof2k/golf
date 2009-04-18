/*
 *  RBGolfPin.cpp
 *  golf
 *
 *  Created by Robert Rose on 4/17/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "RBGolfPin.h"
#include "RudeGL.h"

const float kGolfPinHeight = 10.0f;
const float kGolfPinFlagHeight = 2.0f;

const unsigned int kGolfPinFlagColor = 0xFF0000FF;
const unsigned int kGolfPinColor = 0xFFFFFFFF;

const float kGolfPinFadeIn = 30.0f;
const float kGolfPinFadeSlope = 10.0f;

RBGolfPin::RBGolfPin()
{
}

RBGolfPin::~RBGolfPin()
{
}

void RBGolfPin::Render()
{
	RGL.Enable(kBackfaceCull, true);
	
	RudeObject::Render();
	
	btVector3 eye = RGL.GetEye();
	
	btVector3 eyetopin = eye - m_position;
	
	float dist = eyetopin.length();
	
	if(dist > kGolfPinFadeIn)
	{
		float slope = dist - kGolfPinFadeIn;
		slope /= kGolfPinFadeSlope;
		
		if(slope > 1.0f)
			slope = 1.0f;
		
		RGL.LoadIdentity();
		
		RGL.Enable(kDepthTest, true);
		
		glDisable(GL_TEXTURE_2D);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		
		GLfloat point[] = {
			m_position.x(), m_position.y() + kGolfPinHeight, m_position.z(),
			m_position.x(), m_position.y() + kGolfPinHeight - kGolfPinFlagHeight, m_position.z(),
			m_position.x(), m_position.y() + kGolfPinHeight - kGolfPinFlagHeight, m_position.z(),
			m_position.x(), m_position.y(), m_position.z(),
		};
		
		float colors[] = {
			1.0f, 0.0f, 0.0f, slope,
			1.0f, 0.0f, 0.0f, slope,
			1.0f, 1.0f, 1.0f, slope,
			1.0f, 1.0f, 1.0f, slope
		};
		
		glVertexPointer(3, GL_FLOAT, 0, point);
		glColorPointer(4, GL_FLOAT, 0, colors);
		glDrawArrays(GL_LINES, 0, 4);
	}
}

