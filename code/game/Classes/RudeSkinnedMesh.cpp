/*
 *  RudeSkinnedMesh.cpp
 *  golf
 *
 *  Created by Robert Rose on 9/20/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "RudeSkinnedMesh.h"

#include "RudeGL.h"
#include "RudeTextureManager.h"
#include "RudeFile.h"
#include "RudeDebug.h"

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>



RudeSkinnedMesh::RudeSkinnedMesh(RudeObject *owner)
: RudeMesh(owner)
, m_frame(0.0f)
{
}

RudeSkinnedMesh::~RudeSkinnedMesh()
{
}

int RudeSkinnedMesh::Load(const char *name)
{
	if(RudeMesh::Load(name))
		return -1;
	
	return 0;
	
}

void RudeSkinnedMesh::NextFrame(float delta)
{
	
	m_frame += delta * 30.0f * 0.25f;
	
	if(m_frame > 24.0f)
		m_frame = 0.0f;
	
	m_model.SetFrame(m_frame);
	 
}


void RudeSkinnedMesh::Render()
{
	//int numbonemats;
	//glGetIntegerv(GL_MAX_PALETTE_MATRICES_OES, &numbonemats);
	//printf("bonemats %d\n", numbonemats);
	
	RGL.Enable(kBackfaceCull, true);
	
	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);
	
	
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	//glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	
	glEnable(GL_MATRIX_PALETTE_OES);
	glEnableClientState(GL_MATRIX_INDEX_ARRAY_OES);
	glEnableClientState(GL_WEIGHT_ARRAY_OES);
	
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
		
		RUDE_ASSERT(mesh->sBoneIdx.eType == EPODDataUnsignedByte, "Bone indices must be unsigned byte");
		RUDE_ASSERT(mesh->sBoneWeight.eType == EPODDataFloat, "Bone weight must be float");
		
		
		glMatrixIndexPointerOES(mesh->sBoneIdx.n, GL_UNSIGNED_BYTE, mesh->sBoneIdx.nStride, mesh->pInterleaved + (long) mesh->sBoneIdx.pData);
		glWeightPointerOES(mesh->sBoneWeight.n, GL_FLOAT, mesh->sBoneWeight.nStride, mesh->pInterleaved + (long) mesh->sBoneWeight.pData);
		
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
		
		int totalbatchcnt = 0;
		
		for(int b = 0; b < mesh->sBoneBatches.nBatchCnt; b++)
		{
			int batchcnt = mesh->sBoneBatches.pnBatchBoneCnt[b];

			
			glMatrixMode(GL_MATRIX_PALETTE_OES);
			
			for(int j = 0; j < batchcnt; ++j)
			{
				
				//GL_MATRIX0_ARB
				
				glCurrentPaletteMatrixOES(j);
				
				// Generates the world matrix for the given bone in this batch.
				PVRTMATRIX	mBoneWorld;
				int i32NodeID = mesh->sBoneBatches.pnBatches[j + totalbatchcnt];
				m_model.GetBoneWorldMatrix(mBoneWorld, *node, m_model.pNode[i32NodeID]);
				
				// Multiply the bone's world matrix by the view matrix to put it in view space
				//PVRTMatrixMultiply(mBoneWorld, mBoneWorld, m_mView);
				
				// Load the bone matrix into the current palette matrix.
				glLoadMatrixf(mBoneWorld.f);
				
				
			}
			
			totalbatchcnt += batchcnt;
			
			int offset = mesh->sBoneBatches.pnBatchOffset[b] * 3;
			int end = mesh->sBoneBatches.pnBatchOffset[b+1] * 3;
			
			if(b == (mesh->sBoneBatches.nBatchCnt - 1))
				end = mesh->nNumFaces*3;
			
			int numidx = (end - offset);
			
			glDrawElements(GL_TRIANGLES, numidx, GL_UNSIGNED_SHORT, &indices[offset]);
			
		}
		
		
		
		
	
	}
	
	glDisable(GL_MATRIX_PALETTE_OES);
	glDisableClientState(GL_MATRIX_INDEX_ARRAY_OES);
	glDisableClientState(GL_WEIGHT_ARRAY_OES);
	
}

