/*
 *  RudePhysicsMesh.cpp
 *  golf
 *
 *  Created by Robert Rose on 9/8/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "RudePhysicsMesh.h"
#include "RudeMesh.h"
#include "RudePhysics.h"
#include "RudeDebug.h"

#include <btBulletDynamicsCommon.h>


RudePhysicsMesh::RudePhysicsMesh(RudeObject *owner)
: m_data(0)
, m_shape(0)
, RudePhysicsObject(owner)
{
}

RudePhysicsMesh::~RudePhysicsMesh()
{
	
	if(m_shape)
		delete m_shape;
	if(m_data)
		delete m_data;
}

void RudePhysicsMesh::Load(RudeMesh *mesh, float mass)
{
	CPVRTPODScene *model = mesh->GetModel();
	
	
	m_data = new btTriangleIndexVertexArray();
	
	for(int i = 0; i < model->nNumNode; i++)
	{
		SPODNode *node = &model->pNode[i];
		SPODMesh *mesh = &model->pMesh[node->nIdx];
		
		unsigned char *indices	= (unsigned char*) mesh->sFaces.pData;
		btIndexedMesh btmesh;
		btmesh.m_numTriangles = mesh->nNumFaces;
		btmesh.m_triangleIndexBase = indices;
		btmesh.m_triangleIndexStride = 6;
		btmesh.m_numVertices = mesh->nNumVertex;
		btmesh.m_vertexBase = mesh->pInterleaved + (long)mesh->sVertex.pData;
		btmesh.m_vertexStride = mesh->sVertex.nStride;
		
		m_data->addIndexedMesh(btmesh, PHY_SHORT);
		
		m_materialCodes.push_back(0);
	}
	
	
	m_shape = new btBvhTriangleMeshShape(m_data, false);
	
	
	btVector3 inertia(0,0,0);
	if(mass > 0.0f)
		inertia = btVector3(4,4,4);
	//	m_shape->calculateLocalInertia(mass,inertia);
	
	m_motionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)));
	
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, m_motionState, m_shape, inertia);
	m_rigidBody = new btRigidBody(fallRigidBodyCI);
	
	
	
	RudePhysics::GetInstance()->AddObject(this);
	
}

void RudePhysicsMesh::GetMaterialProperties(int partId, float *friction, float *restitution)
{
	RUDE_ASSERT(partId < m_materialCodes.size(), "Invalid partId");
	
	int materialCode = m_materialCodes[partId];
	cRudePhysicsMeshMaterial &material = m_materials[materialCode];
	RUDE_ASSERT(material.m_valid >= 0, "Invalid material code");
	
	*friction = material.m_friction;
	*restitution = material.m_restitution;
}

