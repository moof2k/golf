/*
 *  RudeSkinnedMesh.cpp
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#include "RudeSkinnedMesh.h"


#include "RudeDebug.h"
#include "RudeGL.h"
#include "RudeFile.h"
#include "RudePerf.h"
#include "RudeTextureManager.h"
#include "RudeTweaker.h"

bool gDebugAnim = false;
RUDE_TWEAK(DebugAnim, kBool, gDebugAnim);

float gDebugAnimFrame = 0;
RUDE_TWEAK(DebugAnimFrame, kFloat, gDebugAnimFrame);

RudeSkinnedMesh::RudeSkinnedMesh(RudeObject *owner)
: RudeMesh(owner)
, m_frame(0.0f)
, m_fps(24.0f)
, m_toFrame(0.0f)
, m_animateTo(false)
{
}

RudeSkinnedMesh::~RudeSkinnedMesh()
{
}

int RudeSkinnedMesh::Load(const char *name)
{
	RUDE_REPORT("RudeSkinnedMesh::Load %s\n", name);
	
	if(RudeMesh::Load(name))
		return -1;
	
	for(unsigned int i = 0; i < m_model.nNumMesh; i++)
	{
		SPODMesh *mesh = &m_model.pMesh[i];
		
		for(int b = 0; b < mesh->sBoneBatches.nBatchCnt; b++)
		{
			int offset = mesh->sBoneBatches.pnBatchOffset[b];
			int end = mesh->sBoneBatches.pnBatchOffset[b+1];
			
			if(b == (mesh->sBoneBatches.nBatchCnt - 1))
				end = mesh->nNumFaces;
			
			int numfaces = (end - offset);
			
			RUDE_REPORT("  Mesh %d-%d: %d tris\n", i, b, numfaces);
		}
		
		RUDE_ASSERT(mesh->sBoneIdx.eType == EPODDataUnsignedByte, "Bone indices must be unsigned byte");
		RUDE_ASSERT(mesh->sBoneWeight.eType == EPODDataFloat, "Bone weight must be float");
		RUDE_ASSERT(mesh->sVertex.eType == EPODDataFloat, "Mesh verts should be float");
	}

	//GLhandleARB g_vertexShader = glCreateShaderObjectARB( GL_VERTEX_SHADER_ARB );
	
	return 0;
	
}

void RudeSkinnedMesh::SetFrame(float f)
{
	m_animateTo = false;
	m_frame = f;
}

void RudeSkinnedMesh::AnimateTo(float f)
{
	m_animateTo = true;
	m_toFrame = f;
}

void RudeSkinnedMesh::NextFrame(float delta)
{
	
	if(m_animateTo)
	{
		if(m_toFrame > m_frame)
		{
			m_frame += delta * m_fps;
			
			if(m_frame > m_toFrame)
			{
				m_frame = m_toFrame;
				m_animateTo = false;
			}
		}
		else
		{
			m_frame -= delta * m_fps;
			
			if(m_frame < m_toFrame)
			{
				m_frame = m_toFrame;
				m_animateTo = false;
			}
		}
		
	}
	
	m_model.SetFrame(m_frame);
	
	if(gDebugAnim)
		m_model.SetFrame(gDebugAnimFrame);
}


void RudeSkinnedMesh::Render()
{
	RUDE_PERF_START(kPerfRudeSkinMeshRender);
	
#ifdef RUDE_PALETTE_MATRIX_SKIN
	
	glMatrixMode(GL_MODELVIEW);
	PVRTMATRIX viewmat;
	glGetFloatv(GL_MODELVIEW_MATRIX, viewmat.f);
	
	RGL.Enable(kBackfaceCull, true);
	
	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);

	RGL.EnableClient(kVertexArray, true);
	RGL.EnableClient(kTextureCoordArray, true);
	
	if(m_animate)
	{
		glEnable(GL_MATRIX_PALETTE_OES);
		glEnableClientState(GL_MATRIX_INDEX_ARRAY_OES);
		glEnableClientState(GL_WEIGHT_ARRAY_OES);
	}
	
	for(int i = 0; i < m_model.nNumNode; i++)
	{
		SPODNode *node = &m_model.pNode[i];
		
		if(!node->pszName)
			continue;
		if(node->pszName[0] != 'M')
			continue;
		
		SPODMaterial *material = &m_model.pMaterial[node->nIdxMaterial];
		SPODMesh *mesh = &m_model.pMesh[node->nIdx];
		
		if(m_animate)
		{
			glMatrixIndexPointerOES(mesh->sBoneIdx.n, GL_UNSIGNED_BYTE, mesh->sBoneIdx.nStride, mesh->pInterleaved + (long) mesh->sBoneIdx.pData);
			glWeightPointerOES(mesh->sBoneWeight.n, GL_FLOAT, mesh->sBoneWeight.nStride, mesh->pInterleaved + (long) mesh->sBoneWeight.pData);
		}
		
		int textureid = material->nIdxTexDiffuse;
		if(textureid >= 0)
			RudeTextureManager::GetInstance()->SetTexture(m_textures[textureid]);
		
		unsigned short *indices	= (unsigned short*) mesh->sFaces.pData;
		
		glVertexPointer(3, GL_FLOAT, mesh->sVertex.nStride, mesh->pInterleaved + (long)mesh->sVertex.pData);
		
		glTexCoordPointer(2, GL_FLOAT, mesh->psUVW->nStride, mesh->pInterleaved + (long)mesh->psUVW->pData);
		
		if((mesh->sVtxColours.n > 0) && (mesh->sVtxColours.eType == EPODDataRGBA))
		{
			RGL.EnableClient(kColorArray, true);
			glColorPointer(4, GL_UNSIGNED_BYTE, mesh->sVtxColours.nStride, mesh->pInterleaved + (long)mesh->sVtxColours.pData);
		}
		else
			RGL.EnableClient(kColorArray, false);
		
		int totalbatchcnt = 0;
		
		for(int b = 0; b < mesh->sBoneBatches.nBatchCnt; b++)
		{
			int batchcnt = mesh->sBoneBatches.pnBatchBoneCnt[b];

			if(m_animate)
			{
				glMatrixMode(GL_MATRIX_PALETTE_OES);
			
				for(int j = 0; j < batchcnt; ++j)
				{
					glCurrentPaletteMatrixOES(j);
					
					// Generates the world matrix for the given bone in this batch.
					PVRTMATRIX	mBoneWorld;
					int i32NodeID = mesh->sBoneBatches.pnBatches[j + totalbatchcnt];
					m_model.GetBoneWorldMatrix(mBoneWorld, *node, m_model.pNode[i32NodeID]);
					
					// Multiply the bone's world matrix by the view matrix to put it in view space
					PVRTMatrixMultiply(mBoneWorld, mBoneWorld, viewmat);
					
					// Load the bone matrix into the current palette matrix.
					glLoadMatrixf(mBoneWorld.f);
				}
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
	
#endif // RUDE_PALETTE_MATRIX_SKIN

#ifdef RUDE_SOFTWARE_SKIN

	
	glMatrixMode(GL_MODELVIEW);
	PVRTMATRIX viewmat;
	glGetFloatv(GL_MODELVIEW_MATRIX, viewmat.f);
	
	RGL.Enable(kBackfaceCull, true);
	
	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);
	
	RGL.EnableClient(kVertexArray, true);
	RGL.EnableClient(kTextureCoordArray, true);
	
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
			RudeTextureManager::GetInstance()->SetTexture(m_textures[textureid]);
		
		unsigned short *indices	= (unsigned short*) mesh->sFaces.pData;
		
		if((mesh->sVtxColours.n > 0) && (mesh->sVtxColours.eType == EPODDataRGBA))
		{
			RGL.EnableClient(kColorArray, true);
			glColorPointer(4, GL_UNSIGNED_BYTE, mesh->sVtxColours.nStride, mesh->pInterleaved + (long)mesh->sVtxColours.pData);
		}
		else
		{
			RGL.EnableClient(kColorArray, false);
			glColor4f(1.0, 1.0, 1.0, 1.0);
		}
		
		int totalbatchcnt = 0;
		
		for(int b = 0; b < mesh->sBoneBatches.nBatchCnt; b++)
		{
			int batchcnt = mesh->sBoneBatches.pnBatchBoneCnt[b];

			btTransform boneworldbt[9];

			if(m_animate)
			{
				for(int j = 0; j < batchcnt; ++j)
				{
					// Get the world matrix for the given bone in this batch.
					
					PVRTMATRIX	boneworld;
					int i32NodeID = mesh->sBoneBatches.pnBatches[j + totalbatchcnt];
					m_model.GetBoneWorldMatrix(boneworld, *node, m_model.pNode[i32NodeID]);

					boneworldbt[j].setFromOpenGLMatrix(boneworld.f);
				}
			}
            else
            {
                for(int j = 0; j < batchcnt; ++j)
				{
                    boneworldbt[j].setIdentity();
                }
            }
			
			totalbatchcnt += batchcnt;
			
			int offset = mesh->sBoneBatches.pnBatchOffset[b] * 3;
			int end = mesh->sBoneBatches.pnBatchOffset[b+1] * 3;
			
			if(b == (mesh->sBoneBatches.nBatchCnt - 1))
				end = mesh->nNumFaces*3;

			for(int v = offset; v < end; v += 3)
			{
				float verts[3][3] = { 0.0 };
				float uvs[3][2] = { 0.0 };

				for(int a = 0; a < 3; a++)
				{
					float *meshverts = (float*) (mesh->pInterleaved + (long)mesh->sVertex.pData + mesh->sVertex.nStride * indices[v+a]);
					float *meshuvs = (float*) (mesh->pInterleaved + (long)mesh->psUVW->pData + mesh->psUVW->nStride * indices[v+a]);
					float *meshweights = (float *) (mesh->pInterleaved + (long)mesh->sBoneWeight.pData + mesh->sBoneWeight.nStride * indices[v+a]);
					unsigned char *meshbones = (unsigned char *) (mesh->pInterleaved + (long)mesh->sBoneIdx.pData + mesh->sBoneIdx.nStride * indices[v+a]);

					btVector3 temppos(0,0,0);
					
					uvs[a][0] = meshuvs[0];
					uvs[a][1] = meshuvs[1];

					for(unsigned int w = 0; w < mesh->sBoneWeight.n; w++)
					{
						float weight = meshweights[w];
						unsigned char boneidx = meshbones[w];

						if(weight < 0.00001f)
							continue;

						btVector3 p(meshverts[0], meshverts[1], meshverts[2]);
						p = boneworldbt[boneidx] * p;
						p = p * weight;
						temppos += p;
					}
					
					verts[a][0] = temppos[0];
					verts[a][1] = temppos[1];
					verts[a][2] = temppos[2];
				}

				glVertexPointer(3, GL_FLOAT, 0, verts);
				glTexCoordPointer(2, GL_FLOAT, 0, uvs);
		
				glDrawArrays(GL_TRIANGLES, 0, 3);
			}
		}
	
	}

#endif // RUDE_SOFTWARE_SKIN

	RUDE_PERF_STOP(kPerfRudeSkinMeshRender);
}
