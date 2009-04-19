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


RBGolfPin::RBGolfPin()
{
}

RBGolfPin::~RBGolfPin()
{
}

void RBGolfPin::Render()
{
	RGL.Enable(kBackfaceCull, true);
	RGL.Enable(kDepthTest, true);
	
	RudeObject::Render();
	

	RGL.LoadIdentity();
	
	RGL.Enable(kDepthTest, false);
	
	glDisable(GL_TEXTURE_2D);
	RGL.EnableClient(kVertexArray, true);
	RGL.EnableClient(kColorArray, true);
	RGL.EnableClient(kTextureCoordArray, false);
	
	GLfloat point[] = {
		m_position.x(), m_position.y() + kGolfPinHeight, m_position.z(),
		m_position.x(), m_position.y() + kGolfPinHeight - kGolfPinFlagHeight, m_position.z(),
		m_position.x(), m_position.y() + kGolfPinHeight - kGolfPinFlagHeight, m_position.z(),
		m_position.x(), m_position.y(), m_position.z(),
	};
	
	unsigned int colors[] = {
		kGolfPinFlagColor,
		kGolfPinFlagColor,
		kGolfPinColor,
		kGolfPinColor
	};
	
	glVertexPointer(3, GL_FLOAT, 0, point);
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, colors);
	glDrawArrays(GL_LINES, 0, 4);

}

