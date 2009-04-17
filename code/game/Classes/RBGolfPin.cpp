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

const float kGolfPinHeight = 8.0f;

const unsigned int kGolfPinColor = 0xFFFFFFFF;

RBGolfPin::RBGolfPin()
{
}

RBGolfPin::~RBGolfPin()
{
}

void RBGolfPin::Render()
{
	RudeObject::Render();
	
	RGL.LoadIdentity();
	
	glDisable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	GLfloat point[] = {
		m_position.x(), m_position.y(), m_position.z(),
		m_position.x(), m_position.y() + kGolfPinHeight, m_position.z(),
	};
	
	unsigned int colors[] = {
		kGolfPinColor,
		kGolfPinColor
	};
	
	glVertexPointer(3, GL_FLOAT, 0, point);
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, colors);
	glDrawArrays(GL_LINES, 0, 4);

}

