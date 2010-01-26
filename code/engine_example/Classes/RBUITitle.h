/*
 *  RBUITitle.h
 *  golf
 *
 *  Created by Robert Rose on 10/29/08.
 *  Copyright 2008 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef __H_RBUITitle
#define __H_RBUITitle

#include "RBGameBase.h"
#include "RBCamera.h"
#include "RudeObject.h"
#include "RudeSkybox.h"
#include "RudeButtonControl.h"
#include "RudeTextControl.h"


class RBUITitle : public RBGameBase {
	
public:
	RBUITitle();
	~RBUITitle();
	
	void NextFrame(float delta);
	void Render(float width, float height);
	
	void TouchDown(RudeTouch *rbt);
	void TouchMove(RudeTouch *rbt);
	void TouchUp(RudeTouch *rbt);
	
	void Reset();
	
private:
	
	RudeTextControl m_text;
	
	float m_timer;
	
	RBCamera m_camera;
	RudeSkybox m_skybox;
	RudeObject m_box;
	RudeObject m_plane;
	RudeObject m_sphere;
};


#endif
