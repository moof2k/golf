/*
 *  RBTerrain.cpp
 *  golf
 *
 *  Created by Robert Rose on 9/8/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "RBTerrain.h"
#include "RudeMesh.h"
#include "RudePhysicsMesh.h"
#include "RudePhysics.h"
#include "RBGolfBall.h"
#include "RudeDebug.h"
#include "RudeTextureManager.h"
#include "RudeGL.h"
#include "RudeTweaker.h"

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>

float gHoleRadius = 0.7f;
float gHoleRenderRadius = 1.0f;
float gHoleAttractDist = 1.1f;
float gHoleAttractPower = 0.1f;
float gHoleMaxBallSpeed = 16.0f;

RUDE_TWEAK(HoleRadius, kFloat, gHoleRadius);
RUDE_TWEAK(HoleRenderRadius, kFloat, gHoleRenderRadius);
RUDE_TWEAK(HoleAttractDist, kFloat, gHoleAttractDist);
RUDE_TWEAK(HoleAttractPower, kFloat, gHoleAttractPower);
RUDE_TWEAK(HoleMaxBallSpeed, kFloat, gHoleMaxBallSpeed);

RBTerrainMaterialInfo gMaterialInfos[kNumMaterialTypes] = {
	{ 2000.0f, 0.3f, 0.97f, 0.9f, 5.0f },
	{ 1000.0f, 0.35f, 0.6f, 0.6f, 3.0f },
	{ 100.0f, 0.4f, 0.4f, 0.4f, 1.0f },
	{ 1000.0f, 0.05f, 0.9f, 0.7f, 10.0f },
	{ 75.0f, 0.4f, 0.5f, 0.5f, 2.0f },
	{ 50.0f, 0.45f, 0.1f, 0.1f, 0.25f },

};

RUDE_TWEAK(MatRoughFriction, kFloat, gMaterialInfos[kRough].m_friction);
RUDE_TWEAK(MatRoughRestitution, kFloat, gMaterialInfos[kRough].m_restitution);
RUDE_TWEAK(MatRoughLinearDamping, kFloat, gMaterialInfos[kRough].m_linearDamping);
RUDE_TWEAK(MatRoughAngularDamping, kFloat, gMaterialInfos[kRough].m_angularDamping);
RUDE_TWEAK(MatRoughMinVelocity, kFloat, gMaterialInfos[kRough].m_minVelocity);

RUDE_TWEAK(MatFairwayFringeFriction, kFloat, gMaterialInfos[kFairwayFringe].m_friction);
RUDE_TWEAK(MatFairwayFringeRestitution, kFloat, gMaterialInfos[kFairwayFringe].m_restitution);
RUDE_TWEAK(MatFairwayFringeLinearDamping, kFloat, gMaterialInfos[kFairwayFringe].m_linearDamping);
RUDE_TWEAK(MatFairwayFringeAngularDamping, kFloat, gMaterialInfos[kFairwayFringe].m_angularDamping);
RUDE_TWEAK(MatFairwayFringeMinVelocity, kFloat, gMaterialInfos[kFairwayFringe].m_minVelocity);

RUDE_TWEAK(MatFairwayFriction, kFloat, gMaterialInfos[kFairway].m_friction);
RUDE_TWEAK(MatFairwayRestitution, kFloat, gMaterialInfos[kFairway].m_restitution);
RUDE_TWEAK(MatFairwayLinearDamping, kFloat, gMaterialInfos[kFairway].m_linearDamping);
RUDE_TWEAK(MatFairwayAngularDamping, kFloat, gMaterialInfos[kFairway].m_angularDamping);
RUDE_TWEAK(MatFairwayMinVelocity, kFloat, gMaterialInfos[kFairway].m_minVelocity);

RUDE_TWEAK(MatSandtrapFriction, kFloat, gMaterialInfos[kSandtrap].m_friction);
RUDE_TWEAK(MatSandtrapRestitution, kFloat, gMaterialInfos[kSandtrap].m_restitution);
RUDE_TWEAK(MatSandtrapLinearDamping, kFloat, gMaterialInfos[kSandtrap].m_linearDamping);
RUDE_TWEAK(MatSandtrapAngularDamping, kFloat, gMaterialInfos[kSandtrap].m_angularDamping);
RUDE_TWEAK(MatSandtrapMinVelocity, kFloat, gMaterialInfos[kSandtrap].m_minVelocity);

RUDE_TWEAK(MatGreenFringeFriction, kFloat, gMaterialInfos[kGreenFringe].m_friction);
RUDE_TWEAK(MatGreenFringeRestitution, kFloat, gMaterialInfos[kGreenFringe].m_restitution);
RUDE_TWEAK(MatGreenFringeLinearDamping, kFloat, gMaterialInfos[kGreenFringe].m_linearDamping);
RUDE_TWEAK(MatGreenFringeAngularDamping, kFloat, gMaterialInfos[kGreenFringe].m_angularDamping);
RUDE_TWEAK(MatGreenFringeMinVelocity, kFloat, gMaterialInfos[kGreenFringe].m_minVelocity);

RUDE_TWEAK(MatGreenFriction, kFloat, gMaterialInfos[kGreen].m_friction);
RUDE_TWEAK(MatGreenRestitution, kFloat, gMaterialInfos[kGreen].m_restitution);
RUDE_TWEAK(MatGreenLinearDamping, kFloat, gMaterialInfos[kGreen].m_linearDamping);
RUDE_TWEAK(MatGreenAngularDamping, kFloat, gMaterialInfos[kGreen].m_angularDamping);
RUDE_TWEAK(MatGreenMinVelocity, kFloat, gMaterialInfos[kGreen].m_minVelocity);

RBTerrain::RBTerrain()
: m_teeBox(1,0,0)
, m_hole(0,0,0)
, m_ballInHole(false)
{
}

void RBTerrainContactCallback(RudePhysicsObject *terrain, RudePhysicsObject *other, int terrainId, int otherId, float *friction, float *restitution)
{
	RBTerrain *tobj = (RBTerrain *) terrain->GetOwner();
	tobj->Contact(other, terrainId, otherId, friction, restitution);
}


void RBTerrain::Load(const char *name)
{
	LoadMesh(name);
	LoadPhysicsMesh(0.0f);
	
	LoadMaterials();
	
	RudePhysicsMesh *obj = (RudePhysicsMesh *) GetPhysicsObject();
	btRigidBody *rb = obj->GetRigidBody();

	rb->setFriction(50.0f);
	rb->setRestitution(0.99f);
	rb->setCollisionFlags(rb->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
	rb->setCollisionFlags(rb->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
	
	obj->SetNotifyOnContact(true);
	obj->SetContactCallback(RBTerrainContactCallback);
	

	LoadNodes();
	
	m_holeTexture = RudeTextureManager::GetInstance()->LoadTextureFromPNGFile("hole");
	RUDE_ASSERT(m_holeTexture, "Could not load hole texture");
	
	
	btDiscreteDynamicsWorld *world = RudePhysics::GetInstance()->GetWorld();
	
	
	btVector3 p0 = m_hole;
	p0.setY(-1000);
	btVector3 p1 = m_hole;
	p1.setY(1000);
	
	btCollisionWorld::ClosestRayResultCallback cb(p0, p1);
	
	world->rayTest(p0, p1, cb);
	
	RUDE_ASSERT(cb.hasHit(), "Could not position hole.. is it over terrain?");
	m_hole = cb.m_hitPointWorld;
}

void RBTerrain::LoadMaterials()
{
	RudeMesh *mesh = (RudeMesh *) GetMesh();
	CPVRTPODScene *scene = mesh->GetModel();
	
	for(int i = 0; i < scene->nNumNode; i++)
	{
		SPODNode *node = &scene->pNode[i];
		
		if(!node->pszName)
			continue;
		if(node->pszName[0] != 'M')
			continue;
		
		switch(node->pszName[1])
		{
			case '0':
				m_terrainParts[i] = kRough;
				break;
			case '1':
				m_terrainParts[i] = kFairwayFringe;
				break;
			case '2':
				m_terrainParts[i] = kFairway;
				break;
			case '3':
				m_terrainParts[i] = kSandtrap;
				break;
			case '4':
				m_terrainParts[i] = kGreenFringe;
				break;
			case '5':
				m_terrainParts[i] = kGreen;
				break;
			default:
				RUDE_ASSERT(0, "Unknown material type on %s", node->pszName);
				break;
				
		}
	}
}

void RBTerrain::LoadNodes()
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
			m_shortTees.push_back(btVector3(node->pfPosition[0], node->pfPosition[1], node->pfPosition[2]));
		else if(node->pszName[1] == '1')
			m_holes.push_back(btVector3(node->pfPosition[0], node->pfPosition[1], node->pfPosition[2]));
		else if(node->pszName[1] == '2')
			m_cameraPlacements.push_back(btVector3(node->pfPosition[0], node->pfPosition[1], node->pfPosition[2]));
		else if(node->pszName[1] == '3')
			m_guidePoints.push_back(btVector3(node->pfPosition[0], node->pfPosition[1], node->pfPosition[2]));
		else if(node->pszName[1] == '4')
			m_longTees.push_back(btVector3(node->pfPosition[0], node->pfPosition[1], node->pfPosition[2]));
	}
	
	
	RUDE_ASSERT(m_shortTees.size() > 0, "Terrain has no short tee boxes (N0)");
	RUDE_ASSERT(m_holes.size() > 0, "Terrain has no holes (N1)");
	RUDE_ASSERT(m_cameraPlacements.size() > 0, "Terrain has no camera placements (N2)");
	
	m_teeBox = m_shortTees[0];
	m_hole = m_holes[0];
	
	//m_hole = m_teeBox + btVector3(0,0,-20);
}

void RBTerrain::Contact(RudePhysicsObject *other, int terrainId, int otherId, float *friction, float *restitution)
{
	eRBTerrainMaterial materialType = m_terrainParts[terrainId];
	RBTerrainMaterialInfo &materialInfo = gMaterialInfos[materialType];
	
	
	*friction = materialInfo.m_friction;
	*restitution = materialInfo.m_restitution;
	
	// assume 'other' is always the ball
	//printf("Contact! %d\n", terrainId);
	
	RBGolfBall *ball = (RBGolfBall *) other->GetOwner();
	btRigidBody *rb = other->GetRigidBody();
	
	// damp based on impacted material (TODO)
	ball->AddContactDamping(materialInfo.m_linearDamping, materialInfo.m_angularDamping);
	
	btVector3 ballpos = ball->GetPosition();
	btVector3 ballvel = rb->getLinearVelocity();
	float ballvelmag = ballvel.length();
	
	// stop based on impacted material (TODO)
	if(ballvelmag < materialInfo.m_minVelocity)
	{
		ball->Stop();
		return;
	}
	
	
	btVector3 holevec = m_hole - ballpos;
	float holedist = holevec.length();
	
	if(holedist < gHoleRadius && ballvelmag < gHoleMaxBallSpeed)
	{
		m_ballInHole = true;
		ball->Stop();
		return;
	}
	
	if(holedist < gHoleAttractDist)
	{
		//printf("balldist %f\n", holedist);
		if(holedist > gHoleRadius)
		{
			holevec.normalize();
			
			//float forcemag = 1.0f / holedist;
			//forcemag *= gHoleAttractPower;
			float forcemag = gHoleAttractPower;
			
			printf("forcemag %f balldist %f ballvel %f\n", forcemag, holedist, ballvelmag);
			
			rb->applyImpulse(holevec * forcemag, btVector3(0,0,0));
		}
	}
}

btVector3 RBTerrain::GetCameraPlacement(btVector3 ball)
{
	int best = -1;
	float bestDistance = 9999.0f;
	
	for(int i = 0; i < m_cameraPlacements.size(); i++)
	{
		btVector3 &p = m_cameraPlacements[i];
		
		btVector3 pvec = p - ball;
		float dist = pvec.length();
		
		if(dist < bestDistance)
		{
			bestDistance = dist;
			best = i;
		}
	}
	
	RUDE_ASSERT(best >= 0, "Could not find a good camera");
	
	return m_cameraPlacements[best];
}

btVector3 RBTerrain::GetGuidePoint(btVector3 ball)
{
	const float kGuideThreshold = 30.0f * 3.0f;
	
	btVector3 holeVec = m_hole - ball;
	float holeDistToBall = holeVec.length();
	
	int guide = -1;
	float bestDistanceHole = holeDistToBall;
	
	for(int i = 0; i < m_guidePoints.size(); i++)
	{
		btVector3 &p = m_guidePoints[i];
		
		btVector3 guideVecBall = p - ball;
		float guideDistToBall = guideVecBall.length();
		
		btVector3 guideVecHole = p - m_hole;
		float guideDistToHole = guideVecHole.length();
		
		if(guideDistToHole < holeDistToBall)
		{
			if(guideDistToBall > kGuideThreshold)
			{
				if(guideDistToHole < bestDistanceHole)
				{
					guide = i;
					bestDistanceHole = guideDistToHole;
				}
			}
		}
		
	}
	
	if(guide >= 0)
		return m_guidePoints[guide];
	else
		return m_hole;
}

void RBTerrain::Render()
{
	RudeObject::Render();
	
	RenderHole();
}

void RBTerrain::RenderHole()
{
	
	RGL.Enable(kDepthTest, false);
	RGL.Enable(kBackfaceCull, false);
	
	RudeTextureManager::GetInstance()->SetTexture(m_holeTexture);
	
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	
	btVector3 &p = m_hole;
	
	const float kHalfHoleSize = gHoleRenderRadius;
	const float kHoleOffset = 0.0f;
	
	GLfloat point[] = {
		p.x() - kHalfHoleSize, p.y() + kHoleOffset, p.z() - kHalfHoleSize,
		p.x() + kHalfHoleSize, p.y() + kHoleOffset, p.z() - kHalfHoleSize,
		p.x() + kHalfHoleSize, p.y() + kHoleOffset, p.z() + kHalfHoleSize,
		p.x() - kHalfHoleSize, p.y() + kHoleOffset, p.z() + kHalfHoleSize,
	};
	
	GLfloat uvs[] = {
		0.0f, 1.0f, 
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
	};
	
	float r,g,b,a;
	r = 0.0f;
	b = 0.0f;
	g = 0.0f;
	a = 1.0f;
	
	GLfloat colors[] = {
		r, g, b, a,
		r, g, b, a,
		r, g, b, a,
		r, g, b, a,
	};
	
	
	glVertexPointer(3, GL_FLOAT, 0, point);
	glColorPointer(4, GL_FLOAT, 0, colors);
	glTexCoordPointer(2, GL_FLOAT, 0, uvs);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

}

