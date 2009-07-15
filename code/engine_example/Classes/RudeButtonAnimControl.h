/*
 *  RudeButtonAnimControl.h
 *  golf
 *
 *  Created by Robert Rose on 4/27/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef __H_RudeButtonAnimControl
#define __H_RudeButtonAnimControl

#include "RudeControl.h"

typedef struct {
	float m_time;
	int m_texture;
} tRudeButtonAnimKeyframe;


class RudeButtonAnimControl : public RudeControl
{
public:
	
	RudeButtonAnimControl();
	
	void SetTextureSize(float size);
	void SetAnimData(tRudeButtonAnimKeyframe *data, int numFrames);
	
	void SetRect(const RudeRect &r);
	
	bool TouchDown(RudeTouch *t);
	bool TouchMove(RudeTouch *t);
	bool TouchUp(RudeTouch *t);
	
	void ResetTimer() { m_time = 0.0f; m_curFrame = 0; }
	
	void Render();
	void NextFrame(float delta);
	
protected:
	
	float m_points[8];
	
	tRudeButtonAnimKeyframe *m_data;
	int m_curFrame;
	
	float m_time;
	
	int m_numFrames;
	int m_texsize;
	int m_halfTexsize;
	
	bool m_state;
	
};

#endif
