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

#include "Rude.h"
#include "RudeGL.h"

#include <vector>

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

const int kMaxInstances = 32;


class RBDecorator
{	
public:
	
	RBDecorator();
	
	void SetTexture(const char *texture);
	void SetSize(float size);
	
	void AddInstance(float x, float y, float z);
	
	void Render();
	
private:
	
	RBDecoratorVert m_verts[6];
	
	RBDecoratorInstance m_instances[kMaxInstances];
	int m_numInstances;
	
	int m_textureid;
};


class RBDecoratorCollection
{
public:

	RBDecoratorCollection();
	
	void Load(const char *deco);
	
	void Render();
	
private:
	
	std::vector<RBDecorator> m_decorators;
	
};


#endif


