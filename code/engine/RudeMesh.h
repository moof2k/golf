/*
 *  RudeMesh.h
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef __H_RudeMesh
#define __H_RudeMesh

#include "PVRTVector.h"
#include "PVRTVertex.h"
#include "PVRTBoneBatch.h"
#include "PVRTModelPOD.h"

#include <btBulletDynamicsCommon.h>

class RudeObject;

const int kMaxTextures = 16;
const int kMaxNodes = 64;

/**
 * Creates a static renderable mesh.\n\n
 *
 * A RudeMesh can contain as many sub-models as you want, but in for them to be renderable they
 * must begin with the 'M' character.\n\n
 *
 * In order to toggle a sub-models renderability using EnableModel(), it must begin with the 'M' character followed by
 * a single number.  I.e., "M0", "M1", etc.  This works by renaming the mesh "m0", "m1", etc.\n\n
 *
 * Do not create a RudeMesh directly.  Instead, create a RudeObject and then call RudeObject::LoadMesh().
 */
class RudeMesh {

public:
	
	RudeMesh(RudeObject *owner);
	virtual ~RudeMesh();
	
	virtual int Load(const char *name);
	
	CPVRTPODScene * GetModel() { return &m_model; }
	
	virtual void NextFrame(float delta) {}
	virtual void Render();
	
	void AddTextureOverride(const char *originalTexture, const char *newTexture);
	void SetColorOverride(int node, const char *colordata);
	
	void SetScale(const btVector3 &s) { m_scale = s; }
	void SetTextureOverride(bool to) { m_textureOverride = to; }
	
	void EnableModel(int n, bool enable);
	
protected:
	
	RudeObject *m_owner;
	
	const char *m_colorOverrides[kMaxNodes];
	
	btVector3 m_scale;
	CPVRTPODScene m_model;
	int m_textures[kMaxTextures];
	int m_textureOverrides[kMaxTextures];
	
	bool m_textureOverride;
	
};


#endif
