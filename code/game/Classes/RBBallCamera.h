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
	kAimCamera,
	kFixedCamera,
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
	
	void NextFrame(float delta);
	
	void Track(eTrackMode mode, btVector3 guide, float height);
	
private:
	
	RBGolfBall *m_ball;
	RBTerrain *m_terrain;
	
	eTrackMode m_mode;
	btVector3 m_guide;
	float m_height;
	float m_desiredHeight;
	float m_yaw;
};


#endif
