/*
 *  RudeSkybox.h
 *  golf
 *
 *  Created by Robert Rose on 9/5/08.
 *  Copyright 2008 Bork 3D LLC. All rights reserved.
 *
 */


#pragma once


class RudeSkybox {
	
public:
	RudeSkybox();
	~RudeSkybox();
	
	int Load(const char *name);
	void Render();
	
private:
	
	float m_verts[24*3];
	float m_uvs[24*2];
	
	int m_textures[6];
	
};


