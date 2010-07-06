/*
 *  RBDecorators.h
 *  golf
 *
 *  Created by Robert Rose on 4/20/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __H_RBDecorators
#define __H_RBDecorators

//#define NO_DECO_EDITOR

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
	float m_uv[2];
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
	int GetTextureID() { return m_textureid; }
	
	void SetSize(float size);
	float GetSize() { return m_size; }
	
	bool AddInstance(float x, float y, float z);
	
	void Print();
	
	void Render();
	
private:
	
	RBDecoratorVert m_verts[6];
	
	RBDecoratorInstance m_instances[kMaxInstances];
	float m_size;
	int m_numInstances;
	
	int m_textureid;
	int m_texturesize;
};


class RBDecoratorCollection
{
public:

	RBDecoratorCollection();
	
	void Load(const char *deco);
	
#ifndef NO_DECO_EDITOR	
	void Drop(const btVector3 &pos, float size);
	void Print();
#endif
	
	void Render();
	
private:
	
	void AddTexture(const char *textureName);
	
	std::vector<RBDecorator> m_decorators;
	
	std::vector<std::string> m_textureNames;
	
#ifndef NO_DECO_EDITOR
	unsigned int m_dropTextureNum;
	std::vector<btVector3> m_droppedDecorators;
#endif
};


#endif


