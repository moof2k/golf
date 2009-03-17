/*
 *  RBGolfClub.cpp
 *  golf
 *
 *  Created by Robert Rose on 9/17/08.
 *  Copyright 2008 Bork 3D LLC. All rights reserved.
 *
 */

#include "RBGolfClub.h"
#include "Rude.h"
#include "RudeSound.h"


// http://www.golfspyder.com/golf-club-loft.html

static RBGolfClub sGolfClubs[kNumGolfClubs] = {
	{ kClubWood,	"1 Wood", "1wood", 200, 240, 11,		kSoundSwingDriver,			kNoSandTrap | kNoFairway | kNoGreen | kNoRough },
	{ kClubWood,	"3 Wood", "3wood", 180, 225, 14,		kSoundSwingWood,			kNoSandTrap | kNoGreen | kNoRough },
	{ kClubWood,	"5 Wood", "5wood", 160, 200, 21,		kSoundSwingWood,			kNoSandTrap | kNoGreen | kNoRough },
	{ kClubIron,	"3 Iron", "3iron", 140, 190, 22,		kSoundSwingIronSoft,		kNoGreen },
	{ kClubIron,	"4 Iron", "4iron", 130, 175, 25.5,		kSoundSwingIronSoft,		kNoGreen },
	{ kClubIron,	"5 Iron", "5iron", 120, 160, 30,		kSoundSwingIronSoft,		kNoGreen },
	{ kClubIron,	"7 Iron", "7iron", 120, 150, 38,		kSoundSwingIronSoft,		kNoGreen },
	{ kClubIron,	"8 Iron", "8iron", 120, 140, 42,		kSoundSwingIronSoft,		kNoGreen },
	{ kClubIron,	"9 Iron", "9iron", 120, 120, 46.5,		kSoundSwingWedge,			kNoGreen }, 
	{ kClubWedge,	"PW", "pw", 90, 90, 50,					kSoundSwingWedge,			kNoGreen },
	{ kClubWedge,	"SW", "sw", 70, 60, 56,					kSoundSwingWedgeInSand,		kNoGreen },
	{ kClubPutter,	"Putter", "putter", 60, 30, 1,			kSoundSwingPutter,			0 }
};


RBGolfClub * RBGolfClub::GetClub(int num)
{
	RUDE_ASSERT(num < kNumGolfClubs, "Invalid club");
	
	return &sGolfClubs[num];
}

bool RBGolfClub::ClubOK(int n, eRBTerrainMaterial curMaterial)
{
	RBGolfClub &club = sGolfClubs[n];
	
	switch(curMaterial)
	{
		case kRough:
			if(club.m_materialRestriction & kNoRough)
				return false;
			break;
		case kFairwayFringe:
		case kFairway:
			if(club.m_materialRestriction & kNoFairway)
				return false;
			break;
		case kSandtrap:
			if(club.m_materialRestriction & kNoSandTrap)
				return false;
			break;
		case kGreenFringe:
		case kGreen:
			if(club.m_materialRestriction & kNoGreen)
				return false;
			break;
	}
	
	return true;

}

int RBGolfClub::NextClub(int curClub, eRBTerrainMaterial curMaterial)
{
	int n = curClub;
	int c = kNumGolfClubs;
	
	while(c > 0)
	{
		n++;
		
		if(n >= kNumGolfClubs)
			n = 0;
		
		if(ClubOK(n, curMaterial))
			return n;
		
		c--;
	}
	
	return curClub;
}

int RBGolfClub::PrevClub(int curClub, eRBTerrainMaterial curMaterial)
{
	int n = curClub;
	int c = kNumGolfClubs;
	
	while(c > 0)
	{
		n--;
		
		if(n < 0)
			n = kNumGolfClubs - 1;
		
		if(ClubOK(n, curMaterial))
			return n;
		
		c--;
	}
	
	return curClub;
}

int RBGolfClub::AutoSelectClub(float yardage, eRBTerrainMaterial curMaterial)
{
	// If you're in a sandtrap return the SW
	if(curMaterial == kSandtrap)
		return 10;
	
	// If you're on the green, return the putter
	if(curMaterial == kGreen)
		return 11;
	if(curMaterial == kGreenFringe)
		return 11;
	
	int bestOvershotClub = -1;
	int bestUndershotClub = -1;
	float bestClubOvershot = 99999.0f;
	float bestClubUndershot = -99999.0f;
	
	// -1: never consider the putter
	
	for(int i = 0; i < kNumGolfClubs - 1; i++)
	{
		RBGolfClub &club = sGolfClubs[i];
		
		if(!ClubOK(i, curMaterial))
			continue;
		
		float overshot = club.m_dist - yardage;
		
		if(overshot > 0.0f)
		{
			if(overshot < bestClubOvershot)
			{
				bestOvershotClub = i;
				bestClubOvershot = overshot;
			}
		}
		else
		{
			if(overshot > bestClubUndershot)
			{
				bestUndershotClub = i;
				bestClubUndershot = overshot;
			}
		}
	}
	
	int bestClub = bestOvershotClub;
	
	if(bestClub < 0)
		bestClub = bestUndershotClub;
	
	RUDE_ASSERT(bestClub >= 0, "Could not find a suitable club w/ yardage %f on material %d", yardage, (int) curMaterial);
	
	return bestClub;
}


