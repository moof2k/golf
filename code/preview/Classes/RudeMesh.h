/*
 *  RudeMesh.h
 *  golf
 *
 *  Created by Robert Rose on 9/4/08.
 *  Copyright 2008 Bork 3D LLC. All rights reserved.
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
