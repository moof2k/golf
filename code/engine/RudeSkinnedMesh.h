/*
 *  RudeSkinnedMesh.h
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef __H_RudeSkinnedMesh
#define __H_RudeSkinnedMesh

#include "Rude.h"
#include "RudeMesh.h"

#if defined(RUDE_WIN) || defined(RUDE_MACOS)
#define RUDE_SOFTWARE_SKIN
#else
#define RUDE_PALETTE_MATRIX_SKIN
#endif

class RudeObject;

/**
 * This class creates a skinned mesh suitable for animation.\n\n
 *
 * Do not create a RudeSkinnedMesh directly.  Instead, create a RudeObject
 * and then call RudeObject::LoadSkinnedMesh().
 */
class RudeSkinnedMesh : public RudeMesh {
	
public:
	
	RudeSkinnedMesh(RudeObject *owner);
	~RudeSkinnedMesh();
	
	virtual int Load(const char *name);
	
	virtual void NextFrame(float delta);
	virtual void Render();

	virtual float GetFrame() { return m_frame; }
	virtual void SetFrame(float f);
	virtual void AnimateTo(float f);
	
	void SetAnimate(bool a) { m_animate = a; }
	
private:

	float m_frame;
	float m_toFrame;
	float m_fps;
	
	bool m_animateTo;
	
	bool m_animate;
	
};


#endif
