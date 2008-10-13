/*
 *  RBGolferObject.h
 *  golf
 *
 *  Created by Robert Rose on 9/17/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __H_RBGolferObject
#define __H_RBGolferObject

#include "RudeObject.h"

const int kNumNodes = 3;

class RBGolferObject : public RudeObject {
	
	
public:
	RBGolferObject();
	
	void Load(const char *mesh);
	
	void SetNode(int i) { m_node = i; }
	
	void SetPosition(btVector3 ball, btVector3 guide)
	{
		m_ball = ball;
		m_guide = guide;
	}
	
	void SetReady();
	void SetBackSwing(float pct);
	
	virtual void Render();
	
private:
	
	void LoadNodes();
	
	btVector3 m_ball;
	btVector3 m_guide;
	
	btVector3 m_nodes[kNumNodes];
	int m_node;
};

#endif
