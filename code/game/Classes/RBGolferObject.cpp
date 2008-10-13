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

RBGolferObject::RBGolferObject()
: m_node(0)
, m_ball(0,0,0)
, m_guide(0,0,1)
{
	for(int i = 0; i < kNumNodes; i++)
		m_nodes[i] = btVector3(0,0,0);
}

void RBGolferObject::Load(const char *mesh)
{
	LoadSkinnedMesh(mesh);
	
	LoadNodes();
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
	
	mesh->SetFrame(1.0f);
}

void RBGolferObject::SetBackSwing(float pct)
{
	RudeSkinnedMesh *mesh = (RudeSkinnedMesh *) GetMesh();
	
	const float kBackSwingStart = 1.0f;
	const float kBackSwingEnd = 32.0f;
	
	float frame = (kBackSwingEnd - kBackSwingStart) * pct + kBackSwingStart;
	
	mesh->SetFrame(frame);
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
	
	glTranslatef(0.0f, 0.0f, -4.0f);
	
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	glScalef(0.04f, 0.04f, 0.04f);

	
	RudeObject::Render();
	 
}





