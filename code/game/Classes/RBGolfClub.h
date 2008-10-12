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

#include "RBTerrainMaterial.h"

const int kNumGolfClubs = 12;

typedef enum {
	kNoSandTrap			= 0x01,
	kNoFairway			= 0x02,
	kNoGreen			= 0x04,
	kNoRough			= 0x08,
	
} eRBGolfClubMaterialRestriction;

class RBGolfClub {
	
public:

	
	const char * m_name;
	const char * m_textureName;
	float m_power;
	float m_dist;
	float m_loft;
	int m_swingsound;
	int m_materialRestriction;
	
	static RBGolfClub * GetClub(int num);
	static int NextClub(int curClub, eRBTerrainMaterial curMaterial);
	static int PrevClub(int curClub, eRBTerrainMaterial curMaterial);
	static int AutoSelectClub(float yardage, eRBTerrainMaterial curMaterial);
	
	static bool ClubOK(int n, eRBTerrainMaterial curMaterial);
	
private:
	
	
	
};


#endif

