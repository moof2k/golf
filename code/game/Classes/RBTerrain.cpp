/*
 *  RBTerrain.cpp
 *  golf
 *
 *  Created by Robert Rose on 9/8/08.
 *  Copyright 2008 Bork 3D LLC. All rights reserved.
 *
 */

#include "RBTerrain.h"
#include "RBGolfBall.h"

#include "RudeColor.h"
#include "RudeCollision.h"
#include "RudeDebug.h"
#include "RudeGL.h"
#include "RudeGLD.h"
#include "RudeMesh.h"
#include "RudePhysicsMesh.h"
#include "RudePhysics.h"
#include "RudeTextureManager.h"
#include "RudeTweaker.h"

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>

bool gDebugRenderGuides = false;

RUDE_TWEAK(DebugRenderGuides, kBool, gDebugRenderGuides);

float gHoleRadius = 0.6f;
float gHoleRenderRadius = 0.8f;
float gHoleAttractDist = 1.1f;
float gHoleAttractPower = 0.1f;
float gHoleMaxBallSpeed = 16.0f;

RUDE_TWEAK(HoleRadius, kFloat, gHoleRadius);
RUDE_TWEAK(HoleRenderRadius, kFloat, gHoleRenderRadius);
RUDE_TWEAK(HoleAttractDist, kFloat, gHoleAttractDist);
RUDE_TWEAK(HoleAttractPower, kFloat, gHoleAttractPower);
RUDE_TWEAK(HoleMaxBallSpeed, kFloat, gHoleMaxBallSpeed);

RBTerrainMaterialInfo gMaterialInfos[kNumMaterialTypes] = {
	{ 2000.0f, 0.3f,	0.8f, 0.95f, 5.0f,		70, 90, 2.0f },
	{ 1000.0f, 0.35f,	0.4f, 0.6f, 3.0f,		95, 100, 1.0f },
	{ 100.0f, 0.4f,		0.35f, 0.45f, 1.0f,		100, 100, 1.0f },
	{ 1000.0f, 0.05f,	0.95f, 0.95f, 10.0f,	50, 65, 1.5f },
	{ 75.0f, 0.4f,		0.25f, 0.5f, 2.0f,		100, 100, 1.0f },
	{ 50.0f, 0.45f,		0.15f, 0.3f, 0.75f,		100, 100, 1.0f },

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
: m_hole(0,0,0)
, m_greenNode(0)
, m_guidePoint(0,0,0)
, m_ballInHole(false)
, m_isPutting(false)
, m_enablePuttingGreen(false)
{
}

void RBTerrainContactCallback(const btVector3 &contactNormal, RudePhysicsObject *terrain, RudePhysicsObject *other, int partId, int otherId, float *friction, float *restitution)
{
	RudePhysicsMesh *physicsmesh = (RudePhysicsMesh *) terrain;
	int terrainId = physicsmesh->GetSubPartMapping(partId);
	
	RBTerrain *tobj = (RBTerrain *) terrain->GetOwner();
	
	tobj->Contact(contactNormal, other, terrainId, otherId, friction, restitution);
}


void RBTerrain::Load(const char *name)
{
	RUDE_REPORT("Loading terrain %s\n", name);
	
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
	p0.setY(m_hole.y() - 1000);
	btVector3 p1 = m_hole;
	p1.setY(m_hole.y() + 1000);
	
	btCollisionWorld::ClosestRayResultCallback cb(p0, p1);
	
	world->rayTest(p0, p1, cb);
	
	RUDE_ASSERT(cb.hasHit(), "Could not position hole.. is it over terrain? (%f %f %f)", m_hole.x(), m_hole.y(), m_hole.z());
	m_hole = cb.m_hitPointWorld;
}

void RBTerrain::LoadMaterials()
{
	RudeMesh *mesh = (RudeMesh *) GetMesh();
	
	mesh->AddTextureOverride("grass_green_A", "grass_green_B");
	
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
				
				if(m_greenNode == 0)
					m_greenNode = i;
				else
				{
					if(strstr(node->pszName, "green") != 0)
						m_greenNode = i;
				}
				
				break;
			default:
				RUDE_ASSERT(0, "Unknown material type on %s", node->pszName);
				break;
				
		}
		
		RUDE_REPORT("LoadMaterials: %s m_terrainParts[%i] = materialType %d\n", node->pszName, i, m_terrainParts[i]);
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
			m_teeBoxes.push_back(btVector3(node->pfPosition[0], node->pfPosition[1], node->pfPosition[2]));
		else if(node->pszName[1] == '1')
			m_holes.push_back(btVector3(node->pfPosition[0], node->pfPosition[1], node->pfPosition[2]));
		else if(node->pszName[1] == '2')
			m_cameraPlacements.push_back(btVector3(node->pfPosition[0], node->pfPosition[1], node->pfPosition[2]));
		else if(node->pszName[1] == '3')
			m_guidePoints.push_back(btVector3(node->pfPosition[0], node->pfPosition[1], node->pfPosition[2]));

	}
	
	
	RUDE_ASSERT(m_teeBoxes.size() > 0, "Terrain has no tee boxes (N0)");
	RUDE_ASSERT(m_holes.size() > 0, "Terrain has no holes (N1)");
	RUDE_ASSERT(m_cameraPlacements.size() > 0, "Terrain has no camera placements (N2)");
	RUDE_ASSERT(m_guidePoints.size() > 0, "Terrain has no guide points (N3)");
	
	m_hole = m_holes[0];
	
	// last guide point is the hole
	m_guidePoints.push_back(m_hole);
}

RBTerrainMaterialInfo & RBTerrain::GetMaterialInfo(eRBTerrainMaterial info)
{
	return gMaterialInfos[info];
}

void RBTerrain::SetPutting(bool isPutting)
{
	m_isPutting = isPutting;
	UpdatePutting();
}

void RBTerrain::SetEnablePuttingGreen(bool enablePuttingGreen)
{
	m_enablePuttingGreen = enablePuttingGreen;
	UpdatePutting();
}

void RBTerrain::UpdatePutting()
{
	RudeMesh *rudemesh = GetMesh();
	
	if(m_isPutting && m_enablePuttingGreen)
	{
		rudemesh->SetTextureOverride(true);
		
		if(m_greenNode)
		{
			CPVRTPODScene *scene = rudemesh->GetModel();
			SPODNode *node = &scene->pNode[m_greenNode];
			SPODMesh *mesh = &scene->pMesh[node->nIdx];
			
			if(mesh->sVtxColours.n > 0)
			{
				RUDE_REPORT("Patching colors...\n");
				
				RUDE_ASSERT(mesh->sVertex.eType == EPODDataFloat, "Expected float vertex data");
				RUDE_ASSERT(mesh->sVtxColours.eType == EPODDataRGBA, "Expected RGBA color data");
				
				for(int i = 0; i < mesh->nNumVertex; i++)
				{
					unsigned char *vertaddr = mesh->pInterleaved + (long)mesh->sVertex.pData + mesh->sVertex.nStride * i;
					unsigned char *coloraddr = mesh->pInterleaved + (long)mesh->sVtxColours.pData + mesh->sVtxColours.nStride * i;
					
					float *vert = (float *) vertaddr;
					unsigned int *color = (unsigned int *) coloraddr;
					
					int color_r = *color & 0xFF;
					int color_g = (*color >> 8) & 0xFF;
					int color_b = (*color >> 16) & 0xFF;
					
					float color_rf = color_r;
					float color_rg = color_g;
					float color_rb = color_b;
					
					color_rf /= 255.0f;
					color_rg /= 255.0f;
					color_rb /= 255.0f;
					
					float verty = vert[1];
					
					
					const float kHeightHighlightScale = 10.0f;
					
					float ydiff = verty - m_ball.y();
					ydiff /= kHeightHighlightScale;
					
					if(ydiff > 1.0f)
						ydiff = 1.0f;
					if(ydiff < -1.0f)
						ydiff = -1.0f;
					
					RudeColorFloat origcolor;
					origcolor.Set(color_rf, color_rg, color_rb);
					
					RudeColorFloat blendcolor;
					
					if(ydiff > 0.0f)
					{
						blendcolor.Set(1.0f, 0.0f, 0.0f);
					}
					else
					{
						blendcolor.Set(0.0f, 0.0f, 1.0f);
						ydiff = -ydiff;
					}
					
					origcolor.Blend(blendcolor, ydiff);
					
					color_r = origcolor.m_r * 255.0f;
					color_g = origcolor.m_g * 255.0f;
					color_b = origcolor.m_b * 255.0f;
					
					unsigned int newcolor = 0xFF000000 |
						color_r |
						color_g << 8 |
						color_b << 16;
					
					m_greenVerts[i] = newcolor;
					
				}
				
				rudemesh->SetColorOverride(m_greenNode, (char *) m_greenVerts);
			}
			
			
		}
	}
	else
	{
		rudemesh->SetColorOverride(m_greenNode, 0);
		
		rudemesh->SetTextureOverride(false);
	}
}

btVector3 RBTerrain::GetTeeBox()
{
	float bestdist = 999999999.0f;
	int best = 0;
	
	if(m_tee == kCourseLongTee)
		bestdist = -1.0f;
	
	for(int i = 0; i < m_teeBoxes.size(); i++)
	{
		btVector3 teevec = m_teeBoxes[i] - m_hole;
		float dist = teevec.length();
		
		if(m_tee == kCourseShortTee)
		{
			if(dist < bestdist)
			{
				bestdist = dist;
				best = i;
			}
		}
		else if(m_tee == kCourseLongTee)
		{
			if(dist > bestdist)
			{
				bestdist = dist;
				best = i;
			}
		}
	}
	
	return m_teeBoxes[best];
}


bool RBTerrain::IsOutOfBounds(const btVector3 &position)
{
	RudePhysicsMesh *obj = (RudePhysicsMesh *) GetPhysicsObject();
	btRigidBody *rb = obj->GetRigidBody();
	
	btDiscreteDynamicsWorld *world = RudePhysics::GetInstance()->GetWorld();
	
	
	btVector3 p0 = position;
	btVector3 p1 = position;
	p1.setY(position.y() - 1000.0f);
	
	RudeRayQueryResultCallback cb(p0, p1);
	world->rayTest(p0, p1, cb);
	
	for(int i = 0; i < cb.m_numRayResults; i++)
	{
		btCollisionObject *obj = cb.m_rayResults[i];
		
		if(obj == rb)
			return false;
	}
	
	return true;
}

bool RBTerrain::IsOutOfBoundsAndGone(const btVector3 &position)
{
	bool isout = IsOutOfBounds(position);
	
	if(isout)
	{
		if(position.y() < -100.0f)
			return true;
	}
	
	return false;
}


void RBTerrain::Contact(const btVector3 &normal, RudePhysicsObject *other, int terrainId, int otherId, float *friction, float *restitution)
{
	
	
	btVector3 up(0,1,0);
	float dampScale = up.dot(normal);
	
	//printf("normal %f %f %f - %f\n", normal.x(), normal.y(), normal.z(), dampScale);
	
	
	eRBTerrainMaterial materialType = m_terrainParts[terrainId];
	RBTerrainMaterialInfo &materialInfo = gMaterialInfos[materialType];
	
	
	*friction = materialInfo.m_friction;
	*restitution = materialInfo.m_restitution;
	
	// assume 'other' is always the ball
	//RUDE_REPORT("Contact! terrainId = %d, materialType = %d\n", terrainId, materialType);
	
	RBGolfBall *ball = (RBGolfBall *) other->GetOwner();
	btRigidBody *rb = other->GetRigidBody();
	
	ball->SetCurMaterial(materialType);
	
	// damp based on impacted material (TODO)
	ball->AddContactDamping(materialInfo.m_linearDamping * dampScale, materialInfo.m_angularDamping * dampScale);
	
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

void RBTerrain::UpdateGuidePoint(const btVector3 &ball, float clubDistance)
{
	m_ball = ball;
	
	// center of circle, flatten to y=0
	btVector3 sc(ball);
	sc.setY(0.0f);
	
	// position of hole, flatten to y=0
	btVector3 hole(m_hole);
	hole.setY(0.0f);
	
	
	bool bestFound = false;
	
	btVector3 bestIntersectionPoint(0,0,0);
	float bestIntersectionDist = 30000.0f;
	
	// #1 search for guide point within range closest to the hole
	
	for(int i = 0; i < m_guidePoints.size(); i++)
	{
		// guide point, flatten to y=0
		btVector3 point(m_guidePoints[i]);
		point.setY(0.0f);
		
		float distToBall = (point - sc).length();
		
		if(distToBall < clubDistance)
		{
			// this point is within range, check how far it is from the hole
			
			float distToHole = (point - hole).length();
			
			if(distToHole < bestIntersectionDist)
			{
				bestIntersectionDist = distToHole;
				bestIntersectionPoint = point;
				bestFound = true;
			}
		}
	}
	
	// #2 search for the sphere/line segment intersection closest to the hole
	
	btVector3 lastpoint;
	
	for(int i = 0; i < m_guidePoints.size(); i++)
	{
		// guide point, flatten to y=0
		btVector3 point(m_guidePoints[i]);
		point.setY(0.0f);
		
		if(i != 0)
		{
			btVector3 p0, p1;
			float p0dist = 30000.0f;
			float p1dist = 30000.0f;
			
			int intersections = RudeCollision::LineSegmentAndSphere(lastpoint, point, sc, clubDistance, p0, p1);
			
			if(intersections)
			{
				p0dist = (p0 - hole).length();
				
				if(intersections > 1)
					p1dist = (p1 - hole).length();
				
				if(p0dist < bestIntersectionDist)
				{
					bestIntersectionDist = p0dist;
					bestIntersectionPoint = p0;
					bestFound = true;
				}
				if(p1dist < bestIntersectionDist)
				{
					bestIntersectionDist = p1dist;
					bestIntersectionPoint = p1;
					bestFound = true;
				}
			}
		}
		
		lastpoint = point;
	}
	
	
	// #3 if we still didn't find a point then just pick the closest point to the ball
	
	if(bestFound == false)
	{
		for(int i = 0; i < m_guidePoints.size(); i++)
		{
			btVector3 point(m_guidePoints[i]);
			point.setY(0.0f);
			
			float distToBall = (point - sc).length();
			
			if(distToBall < bestIntersectionDist)
			{
				bestIntersectionDist = distToBall;
				bestIntersectionPoint = point;
			}
		}
	}
	
	m_guidePoint = bestIntersectionPoint;
}

void RBTerrain::Render()
{
	RudeObject::Render();
	
	RenderHole();
	
	if(gDebugRenderGuides)
		RenderGuides();
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

void RBTerrain::RenderGuides()
{
	btVector3 lastpoint;
	
	for(int i = 0; i < m_guidePoints.size(); i++)
	{
		if(i != 0)
		{
			RGLD.DebugDrawLine(lastpoint, m_guidePoints[i]);
		}
		
		lastpoint = m_guidePoints[i];
	}
	
	btVector3 guide_p1 = m_guidePoint;
	btVector3 guide_p2 = m_guidePoint;
	
	guide_p1.setY(guide_p1.y() - 25);
	guide_p2.setY(guide_p2.y() + 25);
	
	RGLD.DebugDrawLine(guide_p1, guide_p2);
}

bool RBTerrain::CastToTerrain(const btVector3 &start, const btVector3 &end, btVector3 &result)
{
	btVector3 vec = end - start;
	vec *= 1000.0f;
	
	//btVector3 to = p + btVector3(0,-1000,0);
	btDiscreteDynamicsWorld *world = RudePhysics::GetInstance()->GetWorld();
	btCollisionWorld::ClosestRayResultCallback cb(start, vec);
	
	world->rayTest(start, vec, cb);
	
	if(!cb.hasHit())
		return false;

	result = cb.m_hitPointWorld;
	
	//printf("hit %f %f %f\n", r.x(), r.y(), r.z());
	
	return true;
}


