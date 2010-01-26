/*
 *  RBDecorators.h
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef __H_RBDecorators
#define __H_RBDecorators

#include "Rude.h"
#include "RudeGL.h"

#include <vector>
#include <string>

class RBDecoratorVert
{
public:
	RBDecoratorVert()
	{
		m_pos[0] = 0.0f;
		m_pos[1] = 0.0f;
		m_pos[2] = 0.0f;
	}
	
	float m_pos[3];
	short m_uv[2];
};

class RBDecoratorInstance
{
public:
	
	RBDecoratorInstance();
	
	void Set(float x, float y, float z);
	
	float m_pos[3];
};

const int kMaxInstances = 8;


class RBDecorator
{	
public:
	
	RBDecorator();
	
	void SetTexture(const char *texture);
	void SetSize(float size);
	
	void AddInstance(float x, float y, float z);
	
	void Print();
	
	void Render();
	
private:
	
	RBDecoratorVert m_verts[6];
	
	RBDecoratorInstance m_instances[kMaxInstances];
	float m_size;
	int m_numInstances;
	
	int m_textureid;
};


class RBDecoratorCollection
{
public:

	RBDecoratorCollection();
	
	void Load(const char *deco);
	
	void Drop(const btVector3 &pos, float size);
	
	void Render();
	
private:
	
	void AddTexture(const char *textureName);
	
	std::vector<RBDecorator> m_decorators;
	
	std::vector<std::string> m_textureNames;
	int m_dropTextureNum;
};


#endif


