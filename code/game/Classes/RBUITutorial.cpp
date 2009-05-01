/*
 *  RBUITutorial.cpp
 *  golf
 *
 *  Created by Robert Rose on 4/28/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "RBUITutorial.h"
#include "RBCourseData.h"

#include "RudeGL.h"
#include "RudeRegistry.h"
#include "RudeTextureManager.h"

const int kNumTutorialImages = 6;

const char *kTutorialImages[kNumTutorialImages] = {
	"tutorial_a",
	"tutorial_b",
	"tutorial_c",
	"tutorial_d",
	"tutorial_e",
	"tutorial_f"
};

RBUITutorial::RBUITutorial()
: m_course(0)
, m_timer(0)
, m_curImage(-1)
, m_texture(-1)
{
}


void RBUITutorial::NextFrame(float delta)
{
	
	
	if(m_runcheck)
	{
		bool forcetutorial = false;
		RBCourseEntry *course = GetCourseData(m_course);
		
		if(course && course->m_holes == kCourseDrivingRange)
			forcetutorial = true;
		
		RudeRegistry *reg = RudeRegistry::GetSingleton();
		
		bool runtutorial = false;
		
		if(!forcetutorial)
		{
			int loadsize = sizeof(runtutorial);
			if(reg->QueryByte("GOLF", "TUTORIAL", &runtutorial, &loadsize) == 0)
			{
				m_done = true;
				return;
			}
		}
		
		reg->SetByte("GOLF", "TUTORIAL", &runtutorial, sizeof(runtutorial));
		
		m_runcheck = false;
		m_texture = -1;
		m_curImage = -1;
		NextImage();
	}
		
	if(m_imageDelay > 0)
	{
		m_imageDelay--;
		
		if(m_imageDelay == 0)
		{
			if(m_texture >= 0)
			{
				m_texture = RudeTextureManager::GetInstance()->ReplaceTextureFromPNGFile(m_texture, kTutorialImages[m_curImage]);
			}
			else
			{
				m_texture = RudeTextureManager::GetInstance()->LoadTextureFromPNGFile(kTutorialImages[m_curImage]);
			}
		}
	}
	
}

void RBUITutorial::NextImage()
{
	
	m_curImage++;
	
	RUDE_REPORT("Tutorial: Next Image %d\n", m_curImage);
	
	
	if(m_curImage >= kNumTutorialImages)
		m_done = true;
	
	m_imageDelay = 5;
}

void RBUITutorial::Render(float aspect)
{
	RGL.SetViewport(0, 0, 480, 320);
	
	RGL.Ortho(0.0f, 0.0f, 0.0f, 320.0f, 480.0f, 100.0f);
	RGL.LoadIdentity();
	RGL.Enable(kBackfaceCull, false);
	RGL.Enable(kDepthTest, false);

	
	if(m_texture < 0)
		return;
	
	RudeTextureManager::GetInstance()->SetTexture(m_texture);
	
	int xc = (320) / 2;
	int yc = (480) / 2;
	float left = xc - 256;
	float top = yc - 256;
	
	float points[8];
	points[0] = left;
	points[1] = top + 512;
	
	points[2] = left;
	points[3] = top;
	
	points[4] = left + 512;
	points[5] = top;
	
	points[6] = left + 512;
	points[7] = top + 512;
	
	const GLfloat uvs[] = {
		0.0f, 1.0f, 
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
	};
	
	
	
	RGL.EnableClient(kVertexArray, true);
	RGL.EnableClient(kColorArray, false);
	RGL.EnableClient(kTextureCoordArray, true);
	
	glVertexPointer(2, GL_FLOAT, 0, points);
	glTexCoordPointer(2, GL_FLOAT, 0, uvs);
	
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	
}

void RBUITutorial::TouchDown(RudeTouch *rbt)
{
	NextImage();
}

void RBUITutorial::TouchMove(RudeTouch *rbt)
{
}

void RBUITutorial::TouchUp(RudeTouch *rbt)
{
	
}

void RBUITutorial::Reset()
{
	m_done = false;
	m_imageDelay = 0;
	m_curImage = -1;
	m_texture = -1;
	m_timer = 0.0f;
	m_runcheck = true;
}

