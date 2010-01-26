/*
 *  RudeGLD.h
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */


#include "RudeGL.h"

#include <vector>

typedef struct {
	btVector3 p0, p1;
} RudeGLDDebugLine;

/**
 * Debug drawing routines.  Call DebugDraw* repeatedly and then RenderDebug() to flush
 * to the screen.  Useful when you want to draw something on screen but you're not in the
 * middle of a render pass.
 */
class RudeGLD {
	
public:
	
	void DebugDrawLine(btVector3 p0, btVector3 p1);
	
	void RenderDebug();
	
private:
	
	std::vector<RudeGLDDebugLine> m_debugLines;
	
};

extern RudeGLD RGLD;
