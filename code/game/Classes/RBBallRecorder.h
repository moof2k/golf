/*
 *  RBBallRecorder.h
 *  golf
 *
 *  Created by Robert Rose on 10/10/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __H_RBBallRecorder
#define __H_RBBallRecorder


#include "Rude.h"
#include "RBGolfBall.h"

const int kNumBallPositions = 256;

class RBBallRecord {
	
public:
	btVector3 m_position;
	btVector3 m_angVel;
	
	float m_time;
};

class RBBallRecorder {
	
public:
	RBBallRecorder();
	~RBBallRecorder();
	
	void SetBall(RBGolfBall *ball) { m_ball = ball; }
	void Reset();
	void NextFrame(float delta, bool record);
	
	void RenderTracers();
	void RenderRecords();
	
	int GetNumPositions() { return m_curBallPosition; }
	RBBallRecord GetPosition(int i) { return m_ballPositions[i]; }
	
private:
	
	
	RBBallRecord m_ballPositions[kNumBallPositions];
	
	RBGolfBall *m_ball;
	
	float m_timer;
	float m_lastTime;
	bool m_wrapped;
	int m_curBallPosition;
	int m_tracerTexture;
	
};

#endif
