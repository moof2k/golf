/*
 *  RBUITitle.cpp
 *  golf
 *
 *  Created by Robert Rose on 10/29/08.
 *  Copyright 2008 Bork 3D LLC. All rights reserved.
 *
 */

#include "RBUITitle.h"
#include "RudeGL.h"
#include "RudeGLD.h"
#include "RudeTweaker.h"
#include "RudePhysics.h"
#include "RudePhysicsObject.h"
#include "RudeSound.h"



RBUITitle::RBUITitle()
{
	RudePhysics::GetInstance()->Init();
	
	m_skybox.Load("skybox");
	
	m_plane.LoadMesh("plane");
	m_plane.LoadPhysicsMesh(0.0f);
	
	m_box.LoadMesh("box");
	m_box.LoadPhysicsMesh(0.0f);
	
	m_sphere.LoadMesh("sphere");
	m_sphere.LoadPhysicsSphere(1.0f, 1.0f);

	m_text.SetAnimType(kAnimPopSlide);
	m_text.SetText("Bork3D Engine");
	m_text.SetAlignment(RudeTextControl::kAlignCenter);
	m_text.SetRect(RudeRect(0, 0, 240, 320));
	m_text.SetStyle(kOutlineStyle);
	m_text.SetFont(kBigFont);
	m_text.SetColors(0, 0xFFFFFFFF, 0xFFCCCCCC);
	m_text.SetColors(1, 0xFF000000, 0xFF000000);
	
	m_timer = 0.0f;
	
	Reset();
}

RBUITitle::~RBUITitle()
{
}

void RBUITitle::Reset()
{
	
}



void RBUITitle::NextFrame(float delta)
{
	RudePhysics::GetInstance()->NextFrame(delta);
	
	m_timer += delta;
	
	btVector3 lookat(0,0,0);
	
	btVector3 camoff(50,15,0);
	btMatrix3x3 mat;
	mat.setEulerYPR(m_timer * 0.1f, 0.0f, 0.0f);
	camoff = mat * camoff;
	
	btVector3 camera = lookat + camoff;
	
	m_camera.SetPos(camera);
	m_camera.SetLookAt(lookat);
	
	
	
	m_text.NextFrame(delta);
}

void RBUITitle::Render(float width, float height)
{
	float aspect = ((float) width) / ((float) height);
	
	if(RGL.GetLandscape())
		aspect = ((float) height) / ((float) width);
		
	if(RGL.GetLandscape())
		RGL.SetViewport(0, 0, width, height);
	else
		RGL.SetViewport(0, 0, height, width);

	glFrontFace(GL_CW);
	
	// Perspective draws
	m_camera.SetView(aspect);
	RGL.LoadIdentity();
	
	m_skybox.Render();
	
	glCullFace(GL_FRONT);
	
	RGL.Enable(kBackfaceCull, true);
	RGL.Enable(kDepthTest, true);
	
	m_plane.Render();
	m_box.Render();
	m_sphere.Render();

	
	// Ortho draws
	if(RGL.GetLandscape())
		RGL.Ortho(0.0f, 0.0f, 0.0f, height, width, 100.0f);
	else
		RGL.Ortho(0.0f, 0.0f, 0.0f, width, height, 100.0f);
	
	RGL.LoadIdentity();
	RGL.Enable(kBackfaceCull, false);
	RGL.Enable(kDepthTest, false);
	
	m_text.Render();
	
}

void RBUITitle::TouchDown(RudeTouch *rbt)
{
	
}

void RBUITitle::TouchMove(RudeTouch *rbt)
{
}

void RBUITitle::TouchUp(RudeTouch *rbt)
{
	m_text.SetTranslation(btVector3(0,-400,0));
	m_text.SetDesiredTranslation(btVector3(0,0,0));
	
	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(btVector3(0,15,0));
	m_sphere.GetPhysicsObject()->GetRigidBody()->setWorldTransform(trans);
	m_sphere.GetPhysicsObject()->GetRigidBody()->setLinearVelocity(btVector3(0,0,-5));
	m_sphere.GetPhysicsObject()->GetRigidBody()->activate(true);
}

