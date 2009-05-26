/*
 *  RBSwingHelper.cpp
 *  golf
 *
 *  Created by Robert Rose on 4/26/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "RBSwingHelper.h"
#include "RudeGL.h"

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>

const float kSwingTrackStart = 100;
const float kSwingTrackEnd = 400;

const float kHelpInitialDelay = 10.0f;
const float kHelpNextDelay = 3.0f;

RBSwingHelper::RBSwingHelper()
: m_postimer(0.0f)
, m_posdown(true)
, m_enabled(false)
, m_cycling(false)
, m_shortTimer(false)
, m_delaytimer(kHelpInitialDelay)
{
}

void RBSwingHelper::SetEnabled(bool e)
{
	m_enabled = e;
	
	if(m_enabled)
	{
		m_cycling = false;
		
		if(m_shortTimer)
		{
			m_delaytimer = kHelpInitialDelay / 2.0f;
			m_shortTimer = false;
		}
		else
			m_delaytimer = kHelpInitialDelay;
	}
}

void RBSwingHelper::NextFrame(float delta)
{
	if(!m_enabled)
		return;
	
	if(!m_cycling)
	{
		m_delaytimer -= delta;
		
		if(m_delaytimer < 0.0f)
		{
			m_cycling = true;
			m_posdown = true;
			m_postimer = 0.0f;
		}
	}
	
	if(m_cycling)
	{
		if(m_posdown)
			m_postimer += delta;
		else
			m_postimer -= delta * 2.0f;
		
		if(m_postimer > 1.0f)
		{
			m_posdown = false;
		}
		if(m_postimer < -0.2f)
		{
			m_delaytimer = kHelpNextDelay;
			m_cycling = false;
		}
	}
}

void RBSwingHelper::Render()
{
	if(!m_enabled)
		return;
	if(!m_cycling)
		return;
	
	RudeControl::Render();
	
	float yin = 300.0f * m_postimer + kSwingTrackStart;
	float yout = yin - 80;
	
	if(m_posdown == false)
		yout = yin + 80;
	
	if(yout < kSwingTrackStart - 20)
		yout = kSwingTrackStart - 20;
	if(yout > kSwingTrackEnd)
		yout = kSwingTrackEnd;
	
	if(yin > kSwingTrackEnd)
		yin = kSwingTrackEnd;
	if(yin < kSwingTrackStart)
		yin = kSwingTrackStart;
	
	float points[] = {
		0, yin,
		320, yin,
		320, yout,
		0, yout,
	};
	
	unsigned int colors[] = {
		0x7FFFFFFF,
		0x7FFFFFFF,
		0x00FFFFFF,
		0x00FFFFFF
	};
	
	glDisable(GL_TEXTURE_2D);
	RGL.EnableClient(kVertexArray, true);
	RGL.EnableClient(kColorArray, true);
	RGL.EnableClient(kTextureCoordArray, false);
	
	glVertexPointer(2, GL_FLOAT, 0, points);
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, colors);
	
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	
	
}


