/*
 *  RBGolfClub.h
 *  golf
 *
 *  Created by Robert Rose on 9/17/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef __H_RBGolfClub
#define __H_RBGolfClub

const int kNumGolfClubs = 12;

class RBGolfClub {
	
public:

	
	const char * m_name;
	const char * m_textureName;
	float m_power;
	float m_dist;
	float m_loft;
	int m_swingsound;
	
	static RBGolfClub * GetClub(int num);
	static int NextClub(int n);
	static int PrevClub(int n);
	
private:
	
	
	
};


#endif

