/*
 *  RBSwingControl.cpp
 *  golf
 *
 *  Created by Robert Rose on 9/16/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "RBSwingControl.h"
#include "RudeTextureManager.h"
#include "RudeTweaker.h"
#include "RudeFont.h"

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>

float gSwingDownOptimalTimeMin = 1.5f;
float gSwingDownOptimalTimeMax = 1.7f;
float gSwingDownEarlyPunishment = 1.0f;
float gSwingDownLatePunishment = 0.2f;


RUDE_TWEAK(SwingDownOptimalTimeMin, kFloat, gSwingDownOptimalTimeMin);
RUDE_TWEAK(SwingDownOptimalTimeMax, kFloat, gSwingDownOptimalTimeMax);
RUDE_TWEAK(SwingDownEarlyPunishment, kFloat, gSwingDownEarlyPunishment);
RUDE_TWEAK(SwingDownLatePunishment, kFloat, gSwingDownLatePunishment);

const int kSwingTrackStart = 100;
const int kSwingTrackEnd = 400;

const float kSwingDownPrecision = 16.0f;
const float kSwingDownPrecisionPenalty = 0.01f;

const float kSwingUpMaxDeviation = 128.0f;
const float kAngleFromDeviation = 1.0f / kSwingUpMaxDeviation;

RBSwingControl::RBSwingControl()
: m_curSwingPoint(-1)
{
}


void RBSwingControl::Reset()
{
	m_ringTextureId = RudeTextureManager::GetInstance()->LoadTextureFromPNGFile("ring");
	
	m_power = 0.0f;
	m_curSwingPoint = 0;
	m_strokeState = kNoStroke;
	m_downOptimalPct = 0.0f;
	m_upStrokeDeviation = -0.0f;
}

void RBSwingControl::AddSwingPoint(const RudeScreenVertex &p, bool first)
{
	if(m_curSwingPoint == kMaxSwingPoints)
		return;
	
	static mach_timebase_info_data_t    sTimebaseInfo = {0,0};
	if ( sTimebaseInfo.denom == 0 ) {
        (void) mach_timebase_info(&sTimebaseInfo);
    }
	
	if(first)
	{
		m_downTimer = 0.0f;
		
		m_curSwingPoint = 0;
		m_lastPoint = p;
		m_strokeState = kBeginStroke;
		m_downStroke = RudeScreenVertex(0,0);
		m_upStroke = RudeScreenVertex(0,0);
		m_downTime = 0.0f;
		m_upTime = 0.0f;
		m_power = 0.0f;
		m_downPower = 0.0f;
		m_downOptimalPct = 0.0f;
		m_upStrokeDeviation = 0.0f;
		
		m_firstTime = mach_absolute_time();
	}
	
	//printf("Stroke %d %d\n", p.m_x, p.m_y);
	
	if(m_strokeState == kBeginStroke)
	{
		if(p.m_y > m_lastPoint.m_y)
		{
			m_downPower = 0.0f;
			m_strokeState = kDownStroke;
			m_firstTime = mach_absolute_time();
		}
	}
	
	uint64_t thistime = mach_absolute_time();
	uint64_t deltatime = thistime - m_firstTime;
	uint64_t elapsedNano = deltatime * sTimebaseInfo.numer / sTimebaseInfo.denom;
	float elapsedSeconds = ((float) elapsedNano) / 1000000000.0f;
	
	if(m_strokeState == kDownStroke)
	{
		if(p.m_y >= m_lastPoint.m_y)
		{
			m_downTime = elapsedSeconds;
			RudeScreenVertex d = p - m_lastPoint;
			m_downStroke += d;
		}
		else
		{
			m_strokeState = kUpStroke;
		}
	}
	else if(m_strokeState == kUpStroke)
	{
		m_upTime = elapsedSeconds - m_downTime;
		RudeScreenVertex d = p - m_lastPoint;
		m_upStroke += d;
		
		if(m_upStrokeDeviation > 0.0f)
		{
			if(d.m_x > 0)
				m_upStrokeDeviation += d.m_x;
			else
				m_upStrokeDeviation += ((float) d.m_x) * 0.5f;
		}
		else if(m_upStrokeDeviation < 0.0f)
		{
			if(d.m_x < 0)
				m_upStrokeDeviation += d.m_x;
			else
				m_upStrokeDeviation += ((float) d.m_x) * 0.5f;
		}
		else
			m_upStrokeDeviation += d.m_x;
		
		if(m_upStrokeDeviation > kSwingUpMaxDeviation)
			m_upStrokeDeviation = kSwingUpMaxDeviation;
		else if(m_upStrokeDeviation < -kSwingUpMaxDeviation)
			m_upStrokeDeviation = -kSwingUpMaxDeviation;
	}
	
	m_lastPoint = p;
	
	RBSwingPoint sp(p, elapsedSeconds, m_strokeState);
	
	m_swingPoints[m_curSwingPoint] = sp;
	m_curSwingPoint++;
	
}


bool RBSwingControl::TouchDown(RudeTouch *t)
{
	if(!RudeControl::TouchDown(t))
		return false;
	
	AddSwingPoint(t->m_location, true);
	
	return true;
}

bool RBSwingControl::TouchMove(RudeTouch *t)
{
	if(!RudeControl::TouchMove(t))
		return false;
	
	AddSwingPoint(t->m_location, false);
	
	return true;
}

bool RBSwingControl::TouchUp(RudeTouch *t)
{
	if(!RudeControl::TouchUp(t))
		return false;
	
	m_downOptimalPct = 0.0f;
	
	return true;
}


void RBSwingControl::NextFrame(float delta)
{
	if(m_strokeState == kDownStroke)
	{
		m_downTimer += delta;
		
		m_downOptimalPct = m_downTimer / gSwingDownOptimalTimeMin;
		
		if(m_downOptimalPct > 1.0f)
			m_downOptimalPct = 1.0f;
		
		float pathy = kSwingTrackStart + (kSwingTrackEnd - kSwingTrackStart) * m_downOptimalPct;
		
		float lasty = m_lastPoint.m_y;
		
		float diffy = abs(lasty - pathy);
		
		diffy = diffy - kSwingDownPrecision;
		if(diffy < 0.0f)
			diffy = 0.0f;
		
		diffy *= kSwingDownPrecisionPenalty * delta;
		
		m_downPower -= diffy;
		
		m_power = m_downOptimalPct + m_downPower;
		if(m_power < 0.0f)
			m_power = 0.0f;
		
		m_golfer->SetBackSwing(m_power);
		
	}
	else if(m_strokeState == kUpStroke)
	{
		
		
	}
	
}

void RBSwingControl::RenderRing()
{

	RudeFontManager::GetFont(kDefaultFont)->Printf(20.0f, 160.0f, 0.0f, FONT_ALIGN_LEFT, 0xFFFFFFFF, 0xFFFFFF, "PWR %.0f %%", GetPower() * 100.0f);
	
	RudeFontManager::GetFont(kDefaultFont)->Printf(20.0f, 178.0f, 0.0f, FONT_ALIGN_LEFT, 0xFFFFFFFF, 0xFFFFFF, "ANG %.0f %%", GetAngle() * 100.0f);
	
	
	float pathy = kSwingTrackStart + (kSwingTrackEnd - kSwingTrackStart) * m_downOptimalPct;
	
	RudeTextureManager::GetInstance()->SetTexture(m_ringTextureId);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	const float kHalfRingTexSize = 32.0f;
	
	GLfloat point[] = {
		160.0f - kHalfRingTexSize, pathy - kHalfRingTexSize,
		160.0f + kHalfRingTexSize, pathy - kHalfRingTexSize,
		160.0f + kHalfRingTexSize, pathy + kHalfRingTexSize,
		160.0f - kHalfRingTexSize, pathy + kHalfRingTexSize,
	};
	
	GLfloat uvs[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
	};
	
	glVertexPointer(2, GL_FLOAT, 0, point);
	glTexCoordPointer(2, GL_FLOAT, 0, uvs);
	
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	
}


void Blend(float t, float r1, float g1, float b1, float r2, float g2, float b2, float &r, float &g, float &b)
{
	r = (r2 - r1) * t + r1;
	g = (g2 - g1) * t + g1;
	b = (b2 - b1) * t + b1;
}

void PickColor(float t, float &r, float &g, float &b)
{
	if(t < gSwingDownOptimalTimeMin)
		Blend(t * 1.0f / gSwingDownOptimalTimeMin, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, r, g, b);
	else if(t > gSwingDownOptimalTimeMax)
		Blend(1.0f - ((t - gSwingDownOptimalTimeMax) * gSwingDownLatePunishment), 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, r, g, b);
	else
	{
		r = 0.0f;
		b = 0.0f;
		g = 1.0f;
	}
}


void RBSwingControl::RenderTracks()
{
	glDisable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	for(int i = 1; i < m_curSwingPoint; i++)
	{
		RudeScreenVertex &p0 = m_swingPoints[i-1].m_p;
		RudeScreenVertex &p1 = m_swingPoints[i].m_p;
		
		const float kPointSize = 6;
		
		GLfloat point[] = {
			p0.m_x, p0.m_y,
			p0.m_x + kPointSize, p0.m_y,
			p1.m_x + kPointSize, p1.m_y,
			p1.m_x, p1.m_y,
		};
		
		float r1,g1,b1,a1 = 1.0f;
		float r2,g2,b2,a2 = 1.0f;
		
		if(m_swingPoints[i-1].m_state == kDownStroke)
			PickColor(m_swingPoints[i-1].m_time, r1, g1, b1);
		else
		{
			r1 = g1 = b1 = 1.0f;
			a1 = 0.7f;
		}
		
		if(m_swingPoints[i].m_state == kDownStroke)
			PickColor(m_swingPoints[i].m_time, r2, g2, b2);
		else
		{
			r2 = g2 = b2 = 1.0f;
			a2 = 0.7f;
		}
		
		
		GLfloat colors[] = {
			r1, g1, b1, a1,
			r1, g1, b1, a1,
			r2, g2, b2, a2,
			r2, g2, b2, a2,
		};
		
		
		glVertexPointer(2, GL_FLOAT, 0, point);
		
		glColorPointer(4, GL_FLOAT, 0, colors);
		
		
		
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}
	
}

void RBSwingControl::Render()
{
	
	RenderRing();
	
	if(!CanSwing())
		return;
	
	RenderTracks();
}

bool RBSwingControl::CanSwing()
{
	if(m_downTime > gSwingDownOptimalTimeMax)
		return false;
	
	if(m_downStroke.m_y < 10)
		return false;
	
	
	return true;
}

bool RBSwingControl::WillSwing()
{
	if(!CanSwing())
		return false;
	
	if(m_upStroke.m_y > -5)
		return false;
	
	return true;
}

float RBSwingControl::GetPower()
{
	/*
	if(m_downTime < gSwingDownOptimalTimeMin)
		return m_downTime * (1.0f / gSwingDownOptimalTimeMin) * gSwingDownEarlyPunishment;
	
	if(m_downTime > gSwingDownOptimalTimeMax)
	{
		//float power = 1.0f - ((m_downTime - gSwingDownOptimalTimeMax) * gSwingDownLatePunishment);
		//return power;
		return 0.0f;
	}
	
	return 1.0f;
	*/

	return m_power;
}

float RBSwingControl::GetAngle()
{
	float angle = m_upStrokeDeviation * kAngleFromDeviation;
	
	if(angle > 0.0f)
		angle = angle * angle;
	else
		angle = -angle * angle;
	
	return angle;
}

float RBSwingControl::GetImpact()
{
	
	return 0.0f;
}

