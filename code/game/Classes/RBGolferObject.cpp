/*
 *  RBGolferObject.cpp
 *  golf
 *
 *  Created by Robert Rose on 9/17/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "RBGolferObject.h"
#include "RudeMesh.h"
#include "RudeSkinnedMesh.h"

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>

const tRBGolferSwingAnimationPoints kRBGolferSwingAnimationPoints[kNumClubTypes] = {
	{ 0.0f, 13.0f, 13.0f, 19.9f, 32.0f },
	{ 0.0f, 13.0f, 13.0f, 19.9f, 32.0f },
	{ 0.0f, 13.0f, 13.0f, 19.9f, 32.0f },
	{ 39.0f, 46.0f, 46.0f, 51.2f, 76.5f },
};

RBGolferObject::RBGolferObject()
: m_node(0)
, m_ball(0,0,0)
, m_guide(0,0,1)
, m_swingType(kClubPutter)
{
	for(int i = 0; i < kNumNodes; i++)
		m_nodes[i] = btVector3(0,0,0);
}

void RBGolferObject::Load(const char *mesh)
{
	LoadSkinnedMesh(mesh);
	
	LoadNodes();
	
	// force new swing type so we hide the right meshes
	SetSwingType(kClubWood);
}

void RBGolferObject::LoadNodes()
{
	RudeMesh *mesh = (RudeMesh *) GetMesh();
	CPVRTPODScene *scene = mesh->GetModel();
	
	for(int i = 0; i < scene->nNumNode; i++)
	{
		SPODNode *node = &scene->pNode[i];
		
		if(!node->pszName)
			continue;
		if(node->pszName[0] != 'N')
			continue;
		
		if(node->pszName[1] == '0')
			m_nodes[0] = btVector3(node->pfPosition[0], node->pfPosition[1], node->pfPosition[2]);
		else if(node->pszName[1] == '1')
			m_nodes[1] = btVector3(node->pfPosition[0], node->pfPosition[1], node->pfPosition[2]);
		else if(node->pszName[1] == '2')
			m_nodes[2] = btVector3(node->pfPosition[0], node->pfPosition[1], node->pfPosition[2]);
	}
}

void RBGolferObject::SetReady()
{
	RudeSkinnedMesh *mesh = (RudeSkinnedMesh *) GetMesh();
	
	mesh->SetFrame(kRBGolferSwingAnimationPoints[m_swingType].m_backSwingStart);
}

void RBGolferObject::SetRelax()
{
	RudeSkinnedMesh *mesh = (RudeSkinnedMesh *) GetMesh();
	
	const float kBackSwingStart = kRBGolferSwingAnimationPoints[m_swingType].m_backSwingStart;

	mesh->AnimateTo(kBackSwingStart);
}

void RBGolferObject::SetSwingType(eRBGolfClubType type)
{
	if(m_swingType == type)
		return;
	
	m_swingType = type;
	
	SetReady();
	
	RudeSkinnedMesh *mesh = (RudeSkinnedMesh *) GetMesh();
	
	mesh->EnableModel(1, m_swingType == kClubWood);
	mesh->EnableModel(2, m_swingType == kClubIron);
	mesh->EnableModel(3, m_swingType == kClubWedge);
	mesh->EnableModel(4, m_swingType == kClubPutter);
}

void RBGolferObject::SetBackSwing(float pct)
{
	RudeSkinnedMesh *mesh = (RudeSkinnedMesh *) GetMesh();
	
	const float kBackSwingStart = kRBGolferSwingAnimationPoints[m_swingType].m_backSwingStart;
	const float kBackSwingEnd = kRBGolferSwingAnimationPoints[m_swingType].m_backSwingEnd;
	
	float frame = (kBackSwingEnd - kBackSwingStart) * pct + kBackSwingStart;
	
	mesh->SetFrame(frame);
}

void RBGolferObject::SetForwardSwing(float pct)
{
	RudeSkinnedMesh *mesh = (RudeSkinnedMesh *) GetMesh();
	
	const float kForwardSwingStart = kRBGolferSwingAnimationPoints[m_swingType].m_fwdSwingStart;
	const float kForwardContact = kRBGolferSwingAnimationPoints[m_swingType].m_fwdSwingContact;
	const float kForwardSwingEnd = kRBGolferSwingAnimationPoints[m_swingType].m_fwdSwingEnd;
	
	float frame = (kForwardSwingEnd - kForwardContact) * pct + kForwardSwingStart;
	
	mesh->AnimateTo(frame);
}

bool RBGolferObject::HasSwung()
{
	RudeSkinnedMesh *mesh = (RudeSkinnedMesh *) GetMesh();
	
	const float kSwingPoint = kRBGolferSwingAnimationPoints[m_swingType].m_fwdSwingContact;;
	
	float frame = mesh->GetFrame();
	
	if(frame >= kSwingPoint)
		return true;
	else
		return false;
}

void RBGolferObject::Render()
{
	// position golfer relative to ball
	
	btVector3 node = -m_nodes[m_node];
	
	btVector3 guidevec = m_guide - m_ball;
	guidevec.normalize();
	
	float dir = dot(btVector3(0,0,1), guidevec);
	
	float dotp = -dot(btVector3(1,0,0), guidevec);
	
	float yaw = acos(dotp) * 180.0f / 3.1415926f;
	if(dir < 0)
		yaw = -yaw;
	
	yaw += 180.0f;
	
	glTranslatef(m_ball.x(), m_ball.y(), m_ball.z());
	glRotatef(yaw, 0.0f, 1.0f, 0.0f);
	
	//glTranslatef(0.0f, 0.0f, -3.5f);
	
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	//glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	glScalef(0.04f, 0.04f, 0.04f);

	
	RudeObject::Render();
	 
}





