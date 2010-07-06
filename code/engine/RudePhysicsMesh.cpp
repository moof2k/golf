/*
 *  RudePhysicsMesh.cpp
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
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

void RudePhysicsMesh::Load(RudeMesh *modelmesh, float mass)
{
	CPVRTPODScene *model = modelmesh->GetModel();
	
	
	m_data = new btTriangleIndexVertexArray();
	
	for(unsigned int i = 0; i < model->nNumNode; i++)
	{
		SPODNode *node = &model->pNode[i];
		SPODMesh *mesh = &model->pMesh[node->nIdx];
		
		if(!node->pszName)
			continue;
		if(node->pszName[0] != 'M')
			continue;
		
		unsigned char *indices	= (unsigned char*) mesh->sFaces.pData;
		btIndexedMesh btmesh;
		btmesh.m_numTriangles = mesh->nNumFaces;
		btmesh.m_triangleIndexBase = indices;
		btmesh.m_triangleIndexStride = 6;
		btmesh.m_numVertices = mesh->nNumVertex;
		btmesh.m_vertexBase = mesh->pInterleaved + (long)mesh->sVertex.pData;
		btmesh.m_vertexStride = mesh->sVertex.nStride;
		
		/*
		for(int k = 0; k < btmesh.m_numVertices; k++)
		{
			float *base =  (float *) (btmesh.m_vertexBase + btmesh.m_vertexStride * k);
			printf("vert: %f %f %f\n", base[0], base[1], base[2]);
			
		}
		*/
		
		RUDE_REPORT("Loading %s (%d tris, %d verts)\n", node->pszName, btmesh.m_numTriangles, btmesh.m_numVertices);
		
		m_data->addIndexedMesh(btmesh, PHY_SHORT);
		
		int subpart = m_data->getNumSubParts() - 1;
		m_subPartMappings[subpart] = i;
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

