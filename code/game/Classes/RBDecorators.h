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
	
	void Set(const btVector3 &pos, float size);
	
	RBDecoratorVert m_verts[6];
};

const int kMaxInstances = 128;


class RBDecorators
{	
public:
	
	RBDecorators();
	
	void Render();
	
private:
	
	RBDecoratorInstance m_instances[kMaxInstances];
	int m_numInstances;
};

#endif