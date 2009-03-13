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
#include "RBGolfClub.h"

const int kNumNodes = 3;

typedef struct {
	float m_backSwingStart;
	float m_backSwingEnd;
	float m_fwdSwingStart;
	float m_fwdSwingContact;
	float m_fwdSwingEnd;
} tRBGolferSwingAnimationPoints;

class RBGolferObject : public RudeObject
{
	
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
	void SetRelax();
	void SetBackSwing(float pct);
	void SetForwardSwing(float pct);
	void SetSwingType(eRBGolfClubType t);
	bool HasSwung();
	
	virtual void Render();
	
private:
	
	void LoadNodes();
	
	btVector3 m_ball;
	btVector3 m_guide;
	
	btVector3 m_nodes[kNumNodes];
	int m_node;
	
	eRBGolfClubType m_swingType;
};

#endif
