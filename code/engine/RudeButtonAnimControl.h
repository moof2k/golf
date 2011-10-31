/*
 *  RudeButtonAnimControl.h
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef __H_RudeButtonAnimControl
#define __H_RudeButtonAnimControl

#include "RudeButtonControl.h"

#include <vector>

typedef struct {
	float m_time;
	int m_texture;
} tRudeButtonAnimKeyframe;


class RudeButtonAnimControl : public RudeButtonControl
{
public:
	
	RudeButtonAnimControl(RudeControl *parent);
	
	void SetAnimData(const tRudeButtonAnimKeyframe *data, int numFrames);
	
	bool TouchDown(RudeTouch *t);
	bool TouchMove(RudeTouch *t);
	bool TouchUp(RudeTouch *t);
	
	void ResetTimer() { m_time = 0.0f; m_curFrame = 0; }
	
	void Render();
	void NextFrame(float delta);
	
protected:
	
	std::vector<tRudeButtonAnimKeyframe> m_data;
	int m_curFrame;
	
	float m_time;
	
	int m_numFrames;
	
};

#endif
