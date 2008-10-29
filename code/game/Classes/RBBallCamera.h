/*
 *  RBBallCamera.h
 *  golf
 *
 *  Created by Robert Rose on 9/14/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __H_RBBallCamera
#define __H_RBBallCamera

#include "RBCamera.h"
#include "RBGolfBall.h"
#include "RBTerrain.h"

typedef enum {
	kNone,
	kHitCamera,
	kPuttCamera,
	kAimCamera,
	kAfterShotCamera,
	kPlacementCamera,
	kFollowCamera,
	kRegardCamera,
} eTrackMode;

class RBBallCamera : public RBCamera
{
public:
	RBBallCamera();
	
	void SetBall(RBGolfBall *ball) { m_ball = ball; }
	void SetTerrain(RBTerrain *terrain) { m_terrain = terrain; }
	void SetYaw(float f) { m_yaw = f; }
	void SetHeight(float f) { m_height = f; }
	
	void SetDesiredHeight(float f)
	{ 
		m_desiredHeight = f;
		if(m_desiredHeight < 0.0f)
			m_desiredHeight = 0.0f;
		if(m_desiredHeight > 1.0f)
			m_desiredHeight = 1.0f;
	}
	
	void NextFrame(float delta);
	
	void SetTrackMode(eTrackMode mode);
	void SetGuide(const btVector3 &guide) { m_guide = guide; }
	
private:
	
	RBGolfBall *m_ball;
	RBTerrain *m_terrain;
	
	eTrackMode m_mode;
	btVector3 m_guide;
	float m_height;
	float m_desiredHeight;
	float m_yaw;
	bool m_smooth;
};


#endif
