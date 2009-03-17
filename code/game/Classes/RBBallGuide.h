/*
 *  RBBallGuide.h
 *  golf
 *
 *  Created by Robert Rose on 9/16/08.
 *  Copyright 2008 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef __H_RBBallGuide
#define __H_RBBallGuide

#include "RBGolfBall.h"

const int kMaxGuidePoints = 400;

class RBBallGuide
{
public:
	
	RBBallGuide();
	
	void SetBall(RBGolfBall *ball)
	{
		m_ball = ball;
	}
	
	void SetGuide(const btVector3 &guide, bool fullrez)
	{
		m_guide = guide;
		RegenPoints(fullrez);
	}
	
	btVector3 GetLastGuidePoint();
	
	void NextFrame(float delta);
	void Render();
	
private:
	
	void RegenPoints(bool fullrez);
	
	btVector3 m_guidePoints[kMaxGuidePoints];
	int m_numGuidePoints;
	
	RBGolfBall *m_ball;
	btVector3 m_guide;
	
};

#endif
