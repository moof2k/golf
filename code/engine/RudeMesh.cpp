/*
 *  RudeMesh.cpp
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#include "RudeMesh.h"
#include "RudeGL.h"
#include "RudeTextureManager.h"
#include "RudeFile.h"
#include "RudeDebug.h"



RudeMesh::RudeMesh(RudeObject *owner)
: m_owner(owner)
, m_scale(1.0f, 1.0f, 1.0f)
, m_textureOverride(false)
{
	for(int i = 0; i < kMaxNodes; i++)
		m_colorOverrides[i] = 0;
	
	for(int i = 0; i < kMaxTextures; i++)
		m_textureOverrides[i] = -1;
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
	
	RUDE_ASSERT(m_model.nNumTexture < kMaxTextures, "Too many textures in model");
	for(unsigned int i = 0; i < m_model.nNumTexture; i++)
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
	
	// make sure we have at least one renderable node
	bool foundRenderable = false;
	for(unsigned int i = 0; i < m_model.nNumNode; i++)
	{
		SPODNode *node = &m_model.pNode[i];
		
		if(!node->pszName)
			continue;
		if(node->pszName[0] != 'M')
			continue;
		
		foundRenderable = true;
		
		RUDE_REPORT("  Node %s: mesh %d\n", node->pszName, node->nIdx);
	}
	
	RUDE_ASSERT(foundRenderable, "Didn't find any renderable meshes in %s", name);
	
	// flip endianess of colors stored in meshes
	for(unsigned int i = 0; i < m_model.nNumMesh; i++)
	{
		SPODMesh *mesh = &m_model.pMesh[i];
		
		RUDE_ASSERT(mesh->pInterleaved, "Mesh data must be interleaved");
			
		if((mesh->sVtxColours.n > 0))
		{
			RUDE_ASSERT(mesh->sVtxColours.eType == EPODDataRGBA, "Vertex colors must be in RGBA format");
			
			if(mesh->sVtxColours.eType == EPODDataRGBA)
			{
				unsigned char *c = (mesh->pInterleaved + (long)mesh->sVtxColours.pData);
				
				for(unsigned int j = 0; j < mesh->nNumVertex; j++)
				{
					unsigned int *cc = (unsigned int *) c;
					unsigned int b = *cc & 0x000000FF;
					unsigned int g = (*cc & 0x0000FF00) >> 8;
					unsigned int r = (*cc & 0x00FF0000) >> 16;
					//unsigned int a = (*cc & 0xFF000000) >> 24;
					b = g = r;
					
					*cc = 0xFF000000 | (b << 16) | (g << 8) | r;
					
					c += mesh->sVtxColours.nStride;
				}
			}
		}
	}
	
	return 0;
	
}

void RudeMesh::AddTextureOverride(const char *oldTexture, const char *newTexture)
{
	bool found = false;
	
	for(unsigned int i = 0; i < m_model.nNumTexture; i++)
	{
		SPODTexture *texture = &m_model.pTexture[i];
		RUDE_ASSERT(texture, "Invalid texture in model");
		
		char texturename[64];
		sprintf(texturename, "%s", texture->pszName);
		int texturenamelen = strlen(texturename);
		
		// cut off the last 4 chars
		texturename[texturenamelen-4] = '\0';
		
		if(strcmp(oldTexture, texturename) == 0)
		{
			m_textureOverrides[i] = RudeTextureManager::GetInstance()->LoadTextureFromPVRTFile(newTexture);
			RUDE_ASSERT(m_textureOverrides[i] >= 0, "Could not load texture %s", newTexture);
			found = true;
		}
	}

	RUDE_ASSERT(found, "Texture %s not found", oldTexture);
}

void RudeMesh::SetColorOverride(int node, const char *colordata)
{
	RUDE_ASSERT(node < kMaxNodes, "Invalid node");
	
	m_colorOverrides[node] = colordata;
}

void RudeMesh::EnableModel(int n, bool enable)
{
	bool found = false;
	for(unsigned int i = 0; i < m_model.nNumNode; i++)
	{
		SPODNode *node = &m_model.pNode[i];
		
		if(!node->pszName)
			continue;
		
		if(node->pszName[0] == 'M' || node->pszName[0] == 'm')
		{
			if(node->pszName[1] == ('0' + n))
			{
				found = true;
				
				if(enable)
					node->pszName[0] = 'M';
				else
					node->pszName[0] = 'm';
			}
		}
	}
	
	RUDE_ASSERT(found, "Could not find model number %d", n);
	
}

void RudeMesh::Render()
{

	
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	RGL.EnableClient(kVertexArray, true);
	RGL.EnableClient(kTextureCoordArray, true);
	
	//glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	
	//glScalef(m_scale.x(), m_scale.y(), m_scale.z());
	
	for(unsigned int i = 0; i < m_model.nNumNode; i++)
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
		{
			if(m_textureOverride && m_textureOverrides[textureid] >= 0)
				RudeTextureManager::GetInstance()->SetTexture(m_textureOverrides[textureid]);
			else
				RudeTextureManager::GetInstance()->SetTexture(m_textures[textureid]);
		}
		
		unsigned short *indices	= (unsigned short*) mesh->sFaces.pData;
		
		if(mesh->sVertex.eType == EPODDataShortNorm)
		{
			float s = 1.0f / 1000.0f;
			glMatrixMode(GL_MODELVIEW);
			glScalef(s, s, s);
			glVertexPointer(3, GL_UNSIGNED_SHORT, mesh->sVertex.nStride, mesh->pInterleaved + (long)mesh->sVertex.pData);
		}
		else
			glVertexPointer(3, GL_FLOAT, mesh->sVertex.nStride, mesh->pInterleaved + (long)mesh->sVertex.pData);
		
		glTexCoordPointer(2, GL_FLOAT, mesh->psUVW->nStride, mesh->pInterleaved + (long)mesh->psUVW->pData);
		
		if(m_colorOverrides[i])
		{
			RGL.EnableClient(kColorArray, true);
			glColorPointer(4, GL_UNSIGNED_BYTE, 4, m_colorOverrides[i]);
		}
		else
		{
			if(mesh->sVtxColours.n > 0)
			{
				RGL.EnableClient(kColorArray, true);
				glColorPointer(4, GL_UNSIGNED_BYTE, mesh->sVtxColours.nStride, mesh->pInterleaved + (long)mesh->sVtxColours.pData);
			}
			else
			{
				RGL.EnableClient(kColorArray, false);
				glColor4f(1.0, 1.0, 1.0, 1.0);
			}
		}
		
		glDrawElements(GL_TRIANGLES, mesh->nNumFaces*3, GL_UNSIGNED_SHORT, indices);
		
		
	}
	
#if 0
	
	glAlphaFunc ( GL_GREATER, 0.5 ) ;
    glEnable ( GL_ALPHA_TEST ) ;
	
	for(int i = 0; i < m_model.nNumNode; i++)
	{
		SPODNode *node = &m_model.pNode[i];
		
		if(!node->pszName)
			continue;
		if(node->pszName[0] != 'D')
			continue;
		
		SPODMaterial *material = &m_model.pMaterial[node->nIdxMaterial];
		SPODMesh *mesh = &m_model.pMesh[node->nIdx];
		
		int textureid = material->nIdxTexDiffuse;
		if(textureid >= 0)
		{
			if(m_textureOverride && m_textureOverrides[textureid] >= 0)
				RudeTextureManager::GetInstance()->SetTexture(m_textureOverrides[textureid]);
			else
				RudeTextureManager::GetInstance()->SetTexture(m_textures[textureid]);
		}
		
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
		
		if(mesh->sVtxColours.n > 0)
		{
			RGL.EnableClient(kColorArray, true);
			glColorPointer(4, GL_UNSIGNED_BYTE, mesh->sVtxColours.nStride, mesh->pInterleaved + (long)mesh->sVtxColours.pData);
		}
		else
			RGL.EnableClient(kColorArray, false);
			
		glDrawElements(GL_TRIANGLES, mesh->nNumFaces*3, GL_UNSIGNED_SHORT, indices);
		
		
		
	}
	
    glDisable ( GL_ALPHA_TEST ) ;
	
#endif
		
}

