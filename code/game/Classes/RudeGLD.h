/*
 *  RudeGLD.h
 *  golf
 *
 *  Created by Robert Rose on 9/17/08.
 *  Copyright 2008 Bork 3D LLC. All rights reserved.
 *
 */


#include "RudeGL.h"

#include <vector>

typedef struct {
	btVector3 p0, p1;
} RudeGLDDebugLine;

class RudeGLD {
	
public:
	
	void DebugDrawLine(btVector3 p0, btVector3 p1);
	
	void RenderDebug();
	
private:
	
	std::vector<RudeGLDDebugLine> m_debugLines;
	
};

extern RudeGLD RGLD;
