/*
 *  RBDecorators.cpp
 *  golf
 *
 *  Created by Robert Rose on 4/20/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "RBDecorators.h"

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>


RBDecoratorInstance::RBDecoratorInstance()
{
	m_verts[0].m_uv[0] = 0;
	m_verts[0].m_uv[1] = 0;
	
	m_verts[1].m_uv[0] = 1;
	m_verts[1].m_uv[1] = 0;
	
	m_verts[2].m_uv[0] = 0;
	m_verts[2].m_uv[1] = 1;
	
	m_verts[3].m_uv[0] = 0;
	m_verts[3].m_uv[1] = 1;
	
	m_verts[4].m_uv[0] = 1;
	m_verts[4].m_uv[1] = 0;
	
	m_verts[5].m_uv[0] = 1;
	m_verts[5].m_uv[1] = 1;
}

void RBDecoratorInstance::Set(const btVector3 &pos, float size)
{
	float hsize = 0.5f * size;
	
	// bottom left
	m_verts[0].m_pos[0] = pos.x() - hsize;
	m_verts[0].m_pos[1] = pos.y();
	m_verts[0].m_pos[2] = pos.z();
	
	// bottom right
	m_verts[1].m_pos[0] = pos.x() + hsize;
	m_verts[1].m_pos[1] = pos.y();
	m_verts[1].m_pos[2] = pos.z();
	
	// top left
	m_verts[2].m_pos[0] = pos.x() - hsize;
	m_verts[2].m_pos[1] = pos.y() + size;
	m_verts[2].m_pos[2] = pos.z();
	
	// top left
	m_verts[3].m_pos[0] = pos.x() - hsize;
	m_verts[3].m_pos[1] = pos.y() + size;
	m_verts[3].m_pos[2] = pos.z();
	
	// bottom right
	m_verts[4].m_pos[0] = pos.x() + hsize;
	m_verts[4].m_pos[1] = pos.y();
	m_verts[4].m_pos[2] = pos.z();
	
	// top right
	m_verts[5].m_pos[0] = pos.x() + hsize;
	m_verts[5].m_pos[1] = pos.y() + size;
	m_verts[5].m_pos[2] = pos.z();
}

RBDecorators::RBDecorators()
{
	m_numInstances = 3;
	
	m_instances[0].Set(btVector3(0,0,-40), 16);
	
	m_instances[1].Set(btVector3(20,0,-20), 16);
	
	m_instances[2].Set(btVector3(-20,0,-20), 16);
}


void RBDecorators::Render()
{
	RGL.Enable(kBackfaceCull, false);
	
	RGL.EnableClient(kVertexArray, true);
	RGL.EnableClient(kColorArray, false);
	RGL.EnableClient(kTextureCoordArray, true);
	
	btVector3 eye = RGL.GetEye();
	btVector3 forward = RGL.GetForward();
	
	float aspect = 0.666667f;
	float w = 4.587926f;
	RGL.Frustum(0.0f, 0.0f, w * aspect, w, 4.0f, 2000.0f);
	
	

	
	btVector3 z(0,0,1);
	
	float dot = forward.dot(z);
	float angle = acos(dot);
	
	btMatrix3x3 rmat;
	rmat.setEulerYPR(angle, 0.0f, 0.0f); 
	
	//float M[16];
	//rmat.getOpenGLSubMatrix(M);
	
	
	
	
	btVector3 inup(0, 1, 0);
	
	btVector3 side = inup.cross(forward);
	side.normalize();
	
	btVector3 up = forward.cross(side);
	 
	
	
	float M[]= 
	{ 
		side.x(), up.x(), forward.x(), 0, 
		side.y(), up.y(), forward.y(), 0, 
		side.z(), up.z(), forward.z(), 0, 
		0, 0, 0, 1 
	};
	
	for(int i=0; i<3; i+=2 ) 
		for(int j=0; j<3; j++ ) {
			if ( i==j )
				M[i*4+j] = 1.0;
			else
				M[i*4+j] = 0.0;
		}
	
	/*
	float M2[]= 
	{ 
		side.x(), side.y(), side.z(), 0, 
		up.x(), up.y(), up.z(), 0, 
		forward.x(), forward.y(), forward.z(), 0, 
		0, 0, 0, 1 
	};*/
	
	//btMatrix3x3 ymat;
	//ymat.setFromOpenGLSubMatrix(M2);
	//ymat.inverse();
	
	//btMatrix3x3 result = ymat * rmat;
	
	//result.getOpenGLSubMatrix(M);
	//M[12] = 0;
	//M[13] = 0;
	//M[14] = 0;
	//M[15] = 1;
	
	glMatrixMode(GL_PROJECTION);
	glMultMatrixf(M); 
	glTranslatef (-eye.x(), -eye.y(), -eye.z());
	
	
	//RGL.Rotate(angle * 180.0f / 3.14159f, 0.0f, 1.0f, 0.0f);
	
	glVertexPointer(3, GL_FLOAT, sizeof(RBDecoratorVert), &m_instances[0].m_verts[0].m_pos);
	glTexCoordPointer(2, GL_SHORT, sizeof(RBDecoratorVert), &m_instances[0].m_verts[0].m_uv);
	glDrawArrays(GL_TRIANGLES, 0, 6 * m_numInstances);
	
	RGL.LoadIdentity();
}
