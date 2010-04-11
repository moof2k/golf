/*
 *  RudeSkybox.cpp
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#include "RudeSkybox.h"
#include "RudeGL.h"
#include "RudeTextureManager.h"



void SetVertex(float* Vertices, int index, float x, float y, float z)
{
	(Vertices)[index*3+0] = x;
	(Vertices)[index*3+1] = y;
	(Vertices)[index*3+2] = z;
}

void SetUV(float* UVs, int index, float u, float v)
{
	(UVs)[index*2+0] = u;
	(UVs)[index*2+1] = v;
}

RudeSkybox::RudeSkybox()
{
	const float scale = 150.0f;
	const bool adjustUV = true;
	const int textureSize = 512;
	
	float *Vertices = m_verts;
	float *UVs = m_uvs;
	
	float unit = 1.0f;
	float a0 = 0, a1 = unit;
	
	if (adjustUV)
	{
		float oneover = 1.0f / textureSize;
		a0 = 1.0f * oneover;
		a1 = unit - a0;
	}
	
	// Front
	SetVertex(Vertices, 0, -unit, +unit, -unit);
	SetVertex(Vertices, 1, +unit, +unit, -unit);
	SetVertex(Vertices, 2, -unit, -unit, -unit);
	SetVertex(Vertices, 3, +unit, -unit, -unit);
	SetUV(UVs, 0, a0, a1);
	SetUV(UVs, 1, a1, a1);
	SetUV(UVs, 2, a0, a0);
	SetUV(UVs, 3, a1, a0);
	
	// Right
	SetVertex(Vertices, 4, +unit, +unit, -unit);
	SetVertex(Vertices, 5, +unit, +unit, +unit);
	SetVertex(Vertices, 6, +unit, -unit, -unit);
	SetVertex(Vertices, 7, +unit, -unit, +unit);
	SetUV(UVs, 4, a0, a1);
	SetUV(UVs, 5, a1, a1);
	SetUV(UVs, 6, a0, a0);
	SetUV(UVs, 7, a1, a0);
	
	// Back
	SetVertex(Vertices, 8 , +unit, +unit, +unit);
	SetVertex(Vertices, 9 , -unit, +unit, +unit);
	SetVertex(Vertices, 10, +unit, -unit, +unit);
	SetVertex(Vertices, 11, -unit, -unit, +unit);
	SetUV(UVs, 8 , a0, a1);
	SetUV(UVs, 9 , a1, a1);
	SetUV(UVs, 10, a0, a0);
	SetUV(UVs, 11, a1, a0);
	
	// Left
	SetVertex(Vertices, 12, -unit, +unit, +unit);
	SetVertex(Vertices, 13, -unit, +unit, -unit);
	SetVertex(Vertices, 14, -unit, -unit, +unit);
	SetVertex(Vertices, 15, -unit, -unit, -unit);
	SetUV(UVs, 12, a0, a1);
	SetUV(UVs, 13, a1, a1);
	SetUV(UVs, 14, a0, a0);
	SetUV(UVs, 15, a1, a0);
	
	// Top
	SetVertex(Vertices, 16, -unit, +unit, +unit);
	SetVertex(Vertices, 17, +unit, +unit, +unit);
	SetVertex(Vertices, 18, -unit, +unit, -unit);
	SetVertex(Vertices, 19, +unit, +unit, -unit);
	SetUV(UVs, 16, a0, a1);
	SetUV(UVs, 17, a1, a1);
	SetUV(UVs, 18, a0, a0);
	SetUV(UVs, 19, a1, a0);
	
	// Bottom
	SetVertex(Vertices, 20, -unit, -unit, -unit);
	SetVertex(Vertices, 21, +unit, -unit, -unit);
	SetVertex(Vertices, 22, -unit, -unit, +unit);
	SetVertex(Vertices, 23, +unit, -unit, +unit);
	SetUV(UVs, 20, a0, a1);
	SetUV(UVs, 21, a1, a1);
	SetUV(UVs, 22, a0, a0);
	SetUV(UVs, 23, a1, a0);
	
	for (int i=0; i<24*3; i++)
		(Vertices)[i] = (Vertices)[i] * (scale);
	
}


RudeSkybox::~RudeSkybox()
{
}

/**
 * Loads the six skybox textures associated with 'name'.  Skybox textures should be named
 * <name>1.pvr, <name>2.pvr, etc.  For example:\n\n
 *
 * skybox1.pvr\n
 * skybox2.pvr\n
 * ...\n
 * skybox6.pvr
 */
int RudeSkybox::Load(const char *name)
{
	for(int i = 0; i < 6; i++)
	{
		char file[64];
		sprintf(file, "%s%d", name, i+1);
		
		m_textures[i] = RudeTextureManager::GetInstance()->LoadTextureFromPVRTFile(file);
	}
	
	return 0;
}

/**
 * Render the skybox.  Requires the draw state to first be in perspective rendering.
 */
void RudeSkybox::Render()
{
	btVector3 eye = RGL.GetEye();
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(eye.x(), eye.y(), eye.z());
	
	RGL.Enable(kBackfaceCull, true);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
	
	RGL.Enable(kDepthTest, false);
	RGL.EnableClient(kVertexArray, true);
	RGL.EnableClient(kColorArray, true);
	RGL.EnableClient(kTextureCoordArray, true);

	unsigned int colors[] = {
		0xFFFFFFFF,
		0xFFFFFFFF,
		0xFFFFFFFF,
		0xFFFFFFFF
	};

	glColorPointer(4, GL_UNSIGNED_BYTE, 0, colors);
	
	for (int i = 0; i < 6; i++)
	{
		/* Set Data Pointers */
		RudeTextureManager::GetInstance()->SetTexture(m_textures[i]);
		glVertexPointer(3, GL_FLOAT, sizeof(float)*3, &m_verts[i*4*3]);
		glTexCoordPointer(2, GL_FLOAT, sizeof(float)*2, &m_uvs[i*4*2]);
		/* Draw */
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
	
	glPopMatrix();
}


