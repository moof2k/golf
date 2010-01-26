/*
 *  RudeGLD.cpp
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#include "RudeGLD.h"

#include "RudeGL.h"


RudeGLD RGLD;


void RudeGLD::DebugDrawLine(btVector3 p0, btVector3 p1)
{
	RudeGLDDebugLine line;
	
	line.p0 = p0;
	line.p1 = p1;
	
	m_debugLines.push_back(line);
}

void RudeGLD::RenderDebug()
{
#ifdef RUDE_OGLES
	glDisable(GL_TEXTURE_2D);
#endif
    
	RGL.EnableClient(kVertexArray, true);
	RGL.EnableClient(kColorArray, true);
	RGL.EnableClient(kTextureCoordArray, false);
	
	for(int i = 0; i < m_debugLines.size(); i++)
	{
		btVector3 &p0 = m_debugLines[i].p0;
		btVector3 &p1 = m_debugLines[i].p1;
		
		float point[] = {
			p0.x(), p0.y(), p0.z(),
			p1.x(), p1.y(), p1.z(),
			
		};
		
		float colors[] = {
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
		};
		
		
		glVertexPointer(3, GL_FLOAT, 0, point);
		glColorPointer(4, GL_FLOAT, 0, colors);
		
		glDrawArrays(GL_LINES, 0, 2);
	}
	
	m_debugLines.clear();
}


