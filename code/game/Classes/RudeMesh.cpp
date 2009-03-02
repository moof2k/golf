/*
 *  RudeMesh.cpp
 *  golf
 *
 *  Created by Robert Rose on 9/4/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "RudeMesh.h"
#include "RudeGL.h"
#include "RudeTextureManager.h"
#include "RudeFile.h"
#include "RudeDebug.h"

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>



RudeMesh::RudeMesh(RudeObject *owner)
: m_owner(owner)
, m_scale(1.0f, 1.0f, 1.0f)
{
}

RudeMesh::~RudeMesh()
{
}

int RudeMesh::Load(const char *name)
{
	RUDE_ASSERT(name, "Loading mesh with no name");
	
	RUDE_REPORT("RudeMesh::Load %s\n", name);
	
	char filename[64];
	sprintf(filename, "%s.POD", name);
	
	char modelfile[512];
	RudeFileGetFile(filename, modelfile, 512);
	
	int result = m_model.ReadFromFile(modelfile, 0, 0);
	
	RUDE_ASSERT(result == 1, "Could not load model");
	
	if(result == 0)
		return -1;
	
	RUDE_REPORT("RudeMesh::Load 1\n");
	
	RUDE_ASSERT(m_model.nNumTexture < kMaxTextures, "Too many textures in model");
	for(int i = 0; i < m_model.nNumTexture; i++)
	{
		SPODTexture *texture = &m_model.pTexture[i];
		RUDE_ASSERT(texture, "Invalid texture in model");
		
		char texturename[64];
		sprintf(texturename, "%s", texture->pszName);
		int texturenamelen = strlen(texturename);
		
		// cut off the last 4 chars
		texturename[texturenamelen-4] = '\0';
		
		m_textures[i] = RudeTextureManager::GetInstance()->LoadTextureFromPVRTFile(texturename);
		RUDE_ASSERT(m_textures[i] >= 0, "Could not load texture");
		
	}
	
	RUDE_REPORT("RudeMesh::Load 2\n");
	
	// flip endianess of colors stored in meshes
	for(int i = 0; i < m_model.nNumMesh; i++)
	{
		SPODMesh *mesh = &m_model.pMesh[i];
		
		RUDE_ASSERT(mesh->pInterleaved, "Mesh data is assumed to be interleaved");
			
		if((mesh->sVtxColours.n > 0) && (mesh->sVtxColours.eType == EPODDataRGBA))
		{
			unsigned char *c = (mesh->pInterleaved + (long)mesh->sVtxColours.pData);
			
			for(int j = 0; j < mesh->nNumVertex; j++)
			{
				unsigned int *cc = (unsigned int *) c;
				int b = *cc & 0x000000FF;
				int g = (*cc & 0x0000FF00) >> 8;
				int r = (*cc & 0x00FF0000) >> 16;
				
				*cc = 0xFF000000 | (b << 16) | (g << 8) | r;
				
				c += mesh->sVtxColours.nStride;
			}
		}
	}
	
	// make sure we have at least one renderable node
	bool foundRenderable = false;
	for(int i = 0; i < m_model.nNumNode; i++)
	{
		SPODNode *node = &m_model.pNode[i];
		
		if(!node->pszName)
			continue;
		if(node->pszName[0] != 'M')
			continue;
		
		foundRenderable = true;
	}
	
	RUDE_ASSERT(foundRenderable, "Didn't find any renderable meshes in %s", name);
	
	RUDE_REPORT("RudeMesh::Load 3\n");
	
	return 0;
	
}


void RudeMesh::Render()
{

	RGL.Enable(kBackfaceCull, true);
	
	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);

	
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	//glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	
	//glScalef(m_scale.x(), m_scale.y(), m_scale.z());
	
	for(int i = 0; i < m_model.nNumNode; i++)
	{
		SPODNode *node = &m_model.pNode[i];
		
		if(!node->pszName)
			continue;
		if(node->pszName[0] != 'M')
			continue;
		
		SPODMaterial *material = &m_model.pMaterial[node->nIdxMaterial];
		SPODMesh *mesh = &m_model.pMesh[node->nIdx];
		
		int textureid = material->nIdxTexDiffuse;
		if(textureid >= 0)
			RudeTextureManager::GetInstance()->SetTexture(m_textures[textureid]);
		
		unsigned short *indices	= (unsigned short*) mesh->sFaces.pData;
		
		if(mesh->sVertex.eType == EPODDataFixed16_16)
			glVertexPointer(3, GL_FIXED, mesh->sVertex.nStride, mesh->pInterleaved + (long)mesh->sVertex.pData);
		else if(mesh->sVertex.eType == EPODDataShortNorm)
		{
			float s = 1.0f / 1000.0f;
			glMatrixMode(GL_MODELVIEW);
			glScalef(s, s, s);
			glVertexPointer(3, GL_UNSIGNED_SHORT, mesh->sVertex.nStride, mesh->pInterleaved + (long)mesh->sVertex.pData);
		}
		else
			glVertexPointer(3, GL_FLOAT, mesh->sVertex.nStride, mesh->pInterleaved + (long)mesh->sVertex.pData);
		
		glTexCoordPointer(2, GL_FLOAT, mesh->psUVW->nStride, mesh->pInterleaved + (long)mesh->psUVW->pData);
		
		if((mesh->sVtxColours.n > 0) && (mesh->sVtxColours.eType == EPODDataRGBA))
		{
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(4, GL_UNSIGNED_BYTE, mesh->sVtxColours.nStride, mesh->pInterleaved + (long)mesh->sVtxColours.pData);
		}
		else
			glDisableClientState(GL_COLOR_ARRAY);
		
		glDrawElements(GL_TRIANGLES, mesh->nNumFaces*3, GL_UNSIGNED_SHORT, indices);
		
		
	}
	
		
}

