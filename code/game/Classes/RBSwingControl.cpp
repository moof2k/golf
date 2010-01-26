/*
 *  RBSwingControl.cpp
 *  golf
 *
 *  Created by Robert Rose on 9/16/08.
 *  Copyright 2008 Bork 3D LLC. All rights reserved.
 *
 */

#include "RBSwingControl.h"

#include "RudeGL.h"
#include "RudeTextureManager.h"
#include "RudeTweaker.h"
#include "RudeFont.h"

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>

bool gDebugHitPerfect = false;
RUDE_TWEAK(DebugHitPerfect, kBool, gDebugHitPerfect);

float gSwingDownOptimalTimeMin = 1.0f;
float gSwingDownOptimalTimeMax = 2.0f;
float gSwingDownEarlyPunishment = 1.0f;
float gSwingDownLatePunishment = 0.2f;

RUDE_TWEAK(SwingDownOptimalTimeMin, kFloat, gSwingDownOptimalTimeMin);
RUDE_TWEAK(SwingDownOptimalTimeMax, kFloat, gSwingDownOptimalTimeMax);
RUDE_TWEAK(SwingDownEarlyPunishment, kFloat, gSwingDownEarlyPunishment);
RUDE_TWEAK(SwingDownLatePunishment, kFloat, gSwingDownLatePunishment);

const int kSwingTrackStart = 100;
const int kSwingTrackEnd = 400;

const float kSwingBasePower = 1.0f;
const float kSwingTimingBonus = 0.1f;

const float kSwingDownPrecision = 16.0f;
const float kSwingDownPrecisionPenalty = 0.01f;

const float kSwingUpMaxDeviation = 128.0f;
const float kAngleFromDeviation = 1.0f / kSwingUpMaxDeviation;

const float kNoSwingFadeOutTime = 0.5f;
const float kSwingFadeOutTime = 1.5f;

RBSwingControl::RBSwingControl()
: m_curSwingPoint(-1)
, m_renderAlpha(1.0f)
, m_fadeOutTimer(0.0f)
, m_fadeOutTime(kNoSwingFadeOutTime)
, m_noSwingCommentary(false)
{
}


void RBSwingControl::Reset()
{
	m_ringTextureId = RudeTextureManager::GetInstance()->LoadTextureFromPNGFile("ring");
	
	m_curSwingPoint = 0;
	m_strokeState = kNoStroke;
	m_backStrokeAnimDone = true;
	m_downOptimalPct = 0.0f;
	m_upStrokeDeviation = -0.0f;
	
	
	m_swingPowerText.SetAlignment(RudeTextControl::kAlignCenter);
	m_swingPowerText.SetRect(RudeRect(60, 0, 70, 320));
	m_swingPowerText.SetFormat(kIntValue, "  %d %%");
	m_swingPowerText.SetStyle(kOutlineStyle);
	m_swingPowerText.SetFont(kBigFont);
	m_swingPowerText.SetColors(0, 0xFF666666, 0xFF000000);
	m_swingPowerText.SetColors(1, 0xFFCCCCCC, 0xFFFFFFFF);
	
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
		m_downBasePower = 0.0f;
		m_downBonusPower = 0.0f;
		m_downOptimalPct = 0.0f;
		m_upStrokeDeviation = 0.0f;
		
		m_firstTime = mach_absolute_time();
	}
	
	//printf("Stroke %d %d\n", p.m_x, p.m_y);
	
	if(m_strokeState == kBeginStroke)
	{
		if(p.m_y > m_lastPoint.m_y)
		{
			m_downBasePower = 0.0f;
			m_downBonusPower = 0.0f;
			m_strokeState = kDownStroke;
			m_backStrokeAnimDone = false;
			m_fwdStrokeAnimSent = false;
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
				m_upStrokeDeviation += ((float) d.m_x) * 0.2f;
		}
		else if(m_upStrokeDeviation < 0.0f)
		{
			if(d.m_x < 0)
				m_upStrokeDeviation += d.m_x;
			else
				m_upStrokeDeviation += ((float) d.m_x) * 0.2f;
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
	
	AddSwingPoint(t->m_location, false);
	
	if(m_strokeState == kUpStroke)
	{
		if(m_upStroke.m_y < -5)
		{
			//printf("Stroke complete\n");
			m_strokeState = kStrokeComplete;
		}
		else
			m_strokeState = kNoStroke;
	}
	else
		m_strokeState = kNoStroke;
	
	// relax golfer if we didn't take a stroke
	if(m_strokeState == kNoStroke)
	{
		m_golfer->SetRelax();
	}
	
	// fade out if we're done with a stroke
	if(m_strokeState == kNoStroke)
	{
		m_fadeOutTime = kNoSwingFadeOutTime;
		m_fadeOutTimer = m_fadeOutTime;
	}
	if(m_strokeState == kStrokeComplete)
	{
		m_fadeOutTime = kSwingFadeOutTime;
		m_fadeOutTimer = m_fadeOutTime;
	}
	
	m_downOptimalPct = 0.0f;
	
	return true;
}


void RBSwingControl::NextFrame(float delta)
{

	if(m_strokeState == kDownStroke || m_strokeState == kUpStroke || m_strokeState == kStrokeComplete)
	{
		if(!m_backStrokeAnimDone)
		{
			m_downTimer += delta;
			m_downOptimalPct = m_downTimer / gSwingDownOptimalTimeMin;
			
			if(m_downOptimalPct > 1.0f)
			{
				m_backStrokeAnimDone = true;
				m_fwdStrokeAnimSent = false;
				m_downOptimalPct = 1.0f;
			}
			else
			{
				//printf("SetBackSwing %f\n", m_downOptimalPct);
				m_golfer->SetBackSwing(m_downOptimalPct);
			}
		}
		else
		{
			if(m_strokeState == kStrokeComplete)
			{
				if(!m_fwdStrokeAnimSent)
				{
					//printf("SetForwardSwing %f\n", GetPower());
					m_golfer->SetForwardSwing(1.0f);
					m_fwdStrokeAnimSent = true;
				}
			}
		}
		
	}
	
	if(m_strokeState == kDownStroke)
	{
		
		float pathy = kSwingTrackStart + (kSwingTrackEnd - kSwingTrackStart) * m_downOptimalPct;
		
		float lasty = m_lastPoint.m_y;
		
		float diffy = abs(lasty - pathy);
		
		diffy = diffy - kSwingDownPrecision;
		if(diffy < 0.0f)
			diffy = 0.0f;
		
		diffy *= kSwingDownPrecisionPenalty * delta;
		
		m_downBonusPower -= diffy;
		
		//m_power = m_downOptimalPct;
		
		m_downBasePower = ((float) m_downStroke.m_y) / ((float) kSwingTrackEnd - kSwingTrackStart);
		
		if(m_downBasePower < 0.0f)
			m_downBasePower = 0.0f;
		if(m_downBasePower > kSwingBasePower)
			m_downBasePower = kSwingBasePower;
		
		//printf("base = %f  bonus = %f\n", m_downBasePower, m_downBonusPower);
		
	}
	else if(m_strokeState == kUpStroke)
	{
		
	}
	else if(m_strokeState == kStrokeComplete)
	{
		
	}
	
	if(m_strokeState == kStrokeComplete || m_strokeState == kNoStroke)
	{
		if(m_fadeOutTimer > 0.0f)
		{
			m_fadeOutTimer -= delta;
		}
	}
	else
		m_fadeOutTimer = m_fadeOutTime;
}

void RBSwingControl::RenderRing()
{
	

	RudeTextureManager::GetInstance()->SetTexture(m_ringTextureId);
	
	RGL.EnableClient(kVertexArray, true);
	RGL.EnableClient(kColorArray, true);
	RGL.EnableClient(kTextureCoordArray, true);
	
	
	float ringsize = (1.0f - m_downOptimalPct) * (kSwingTrackEnd - kSwingTrackStart) + 32.0f;
	float pathy = kSwingTrackEnd;
	
	GLfloat point[] = {
		160.0f - ringsize, pathy - ringsize,
		160.0f + ringsize, pathy - ringsize,
		160.0f + ringsize, pathy + ringsize,
		160.0f - ringsize, pathy + ringsize,
	};
	
	GLfloat uvs[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
	};
	
	float alpha = m_downOptimalPct;
	
	GLfloat colors[] = {
		1.0f, 1.0f, 1.0f, alpha,
		1.0f, 1.0f, 1.0f, alpha,
		1.0f, 1.0f, 1.0f, alpha,
		1.0f, 1.0f, 1.0f, alpha,
	};
	
	glVertexPointer(2, GL_FLOAT, 0, point);
	glColorPointer(4, GL_FLOAT, 0, colors);
	glTexCoordPointer(2, GL_FLOAT, 0, uvs);
	
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	 
	
	
	{
		float ringsize = 32.0f;
		float pathy = kSwingTrackStart;
		
		GLfloat point[] = {
			160.0f - ringsize, pathy - ringsize,
			160.0f + ringsize, pathy - ringsize,
			160.0f + ringsize, pathy + ringsize,
			160.0f - ringsize, pathy + ringsize,
		};
		
		GLfloat uvs[] = {
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f,
		};
		
		float alpha = 1.0f;
		
		GLfloat colors[] = {
			1.0f, 1.0f, 1.0f, alpha,
			1.0f, 1.0f, 1.0f, alpha,
			1.0f, 1.0f, 1.0f, alpha,
			1.0f, 1.0f, 1.0f, alpha,
		};
		
		glVertexPointer(2, GL_FLOAT, 0, point);
		glColorPointer(4, GL_FLOAT, 0, colors);
		glTexCoordPointer(2, GL_FLOAT, 0, uvs);
		
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}

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

void RBSwingControl::RenderPower()
{
	float power = GetPower();

	float totalAlpha = m_fadeOutTimer / m_fadeOutTime;
	
	glDisable(GL_TEXTURE_2D);
	RGL.EnableClient(kVertexArray, true);
	RGL.EnableClient(kColorArray, true);
	RGL.EnableClient(kTextureCoordArray, false);
	
	float alpha = totalAlpha * 0.8f * power;
	
	if(alpha > 1.0f)
		alpha = 1.0f;
	
	GLfloat colors[] = {
		1.0f, 1.0f, 1.0f, alpha,
		1.0f, 1.0f, 1.0f, alpha,
		1.0f, 1.0f, 1.0f, alpha,
		1.0f, 1.0f, 1.0f, alpha,
	};
	
	const float kY = 46;
	const float kY2 = 68;
	const float kX = 160;
	const float kLen = 140;
	
	float w = power * kLen;
	
	GLfloat point[] = {
		kX - w, kY,
		kX + w, kY,
		kX + w, kY2,
		kX - w, kY2,
	};
	
	glVertexPointer(2, GL_FLOAT, 0, point);
	glColorPointer(4, GL_FLOAT, 0, colors);
	
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	
	
	if(power > 0.0f)
	{
		m_swingPowerText.SetAlpha(2.0f * totalAlpha);
		
		if(m_noSwingCommentary || m_strokeState != kStrokeComplete)
		{
			m_swingPowerText.SetValue(power * 100.0f);
		}
		else
		{
			if(power > 1.9f)
			{
				m_swingPowerText.SetText("Excellent Shot!");
			}
			else if(power > 1.5f)
			{
				m_swingPowerText.SetText("Great Shot!");
			}
			else if(power > 1.0f)
			{
				m_swingPowerText.SetText("Good Shot!");
			}
			else
			{
				m_swingPowerText.SetValue(power * 100.0f);
			}
		}
		
		m_swingPowerText.Render();
	}
}


void RBSwingControl::RenderTracks()
{
	glDisable(GL_TEXTURE_2D);
	RGL.EnableClient(kVertexArray, true);
	RGL.EnableClient(kColorArray, true);
	RGL.EnableClient(kTextureCoordArray, false);
	
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
	RenderPower();
	//RenderRing();
	
	if(!CanSwing())
		return;
	
	//RenderTracks();
}

bool RBSwingControl::CanSwing()
{
	if(m_strokeState == kNoStroke)
		return false;
	
	return true;
}

bool RBSwingControl::WillSwing()
{
	
	if((m_strokeState == kStrokeComplete) && (m_fwdStrokeAnimSent == true))
	{
		return true;
	}
	
	return false;
}

float RBSwingControl::GetPower()
{
	if(gDebugHitPerfect)
		return 1.0f;
	
	const float kAnglePenaltyModifier = 0.25f;
	
	float anglePenalty = GetAngle();
	if(anglePenalty < 0.0f)
		anglePenalty = -anglePenalty;
	
	anglePenalty = 1.0f - anglePenalty * kAnglePenaltyModifier;
	
	
	float timingBonus = m_downBonusPower + 0.2f;
	timingBonus *= 0.5f;
	
	if(timingBonus > kSwingTimingBonus)
		timingBonus = kSwingTimingBonus;
	if(timingBonus < 0.0f)
		timingBonus = 0.0f;
	
	float power = m_downBasePower + timingBonus;
	
	power = power * anglePenalty;

	return power;
}

float RBSwingControl::GetAngle()
{
	if(gDebugHitPerfect)
		return 0.0f;
	
	float angle = m_upStrokeDeviation * kAngleFromDeviation;
	
	//if(angle > 0.0f)
	//	angle = angle * angle;
	//else
	//	angle = -angle * angle;
	
	return angle;
}

float RBSwingControl::GetImpact()
{
	
	return 0.0f;
}

