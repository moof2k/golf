/*
 *  RudeObject.cpp
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#include "RudeObject.h"
#include "RudeMesh.h"
#include "RudeSkinnedMesh.h"
#include "RudePhysicsMesh.h"
#include "RudePhysicsSphere.h"
#include "RudeGL.h"



RudeObject::RudeObject()
: m_mesh(0)
, m_physics(0)
, m_position(0,0,0)
{
}

RudeObject::~RudeObject()
{
	if(m_mesh)
	{
		delete m_mesh;
		m_mesh = 0;
	}
	if(m_physics)
	{
		delete m_physics;
		m_physics = 0;
	}
}

void RudeObject::LoadMesh(const char *name)
{
	m_mesh = new RudeMesh(this);
	
	m_mesh->Load(name);
}

void RudeObject::LoadSkinnedMesh(const char *name)
{
	m_mesh = new RudeSkinnedMesh(this);
	
	m_mesh->Load(name);
}

void RudeObject::LoadPhysicsMesh(float mass)
{
	m_physics = new RudePhysicsMesh(this);
	((RudePhysicsMesh *)m_physics)->Load(m_mesh, mass);
}

void RudeObject::LoadPhysicsSphere(float radius, float mass)
{
	m_physics = new RudePhysicsSphere(this);
	((RudePhysicsSphere *)m_physics)->Load(radius, mass);
}

void RudeObject::Render()
{
	if(m_physics)
		m_physics->Render();
	else
	{
		RGL.Translate(m_position.x(), m_position.y(), m_position.z());
	}
	
	if(m_mesh)
		m_mesh->Render();
}

void RudeObject::NextFrame(float delta)
{
	if(m_mesh)
		m_mesh->NextFrame(delta);
}


