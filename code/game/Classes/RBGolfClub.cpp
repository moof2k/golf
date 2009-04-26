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
	{ kClubWood,	"1 Wood", "ui_1wood", 223, 240, 11,		kSoundSwingDriver,			kNoSandTrap | kNoFairway | kNoGreen | kNoRough },
	{ kClubWood,	"3 Wood", "ui_3wood", 193, 225, 14,		kSoundSwingWood,			kNoSandTrap | kNoGreen | kNoRough },
	{ kClubWood,	"5 Wood", "ui_5wood", 152, 200, 21,		kSoundSwingWood,			kNoSandTrap | kNoGreen | kNoRough },
	{ kClubIron,	"3 Iron", "ui_3iron", 145, 190, 22,		kSoundSwingIronSoft,		kNoGreen },
	{ kClubIron,	"4 Iron", "ui_4iron", 132, 175, 25.5,	kSoundSwingIronSoft,		kNoGreen },
	{ kClubIron,	"5 Iron", "ui_5iron", 118, 160, 30,		kSoundSwingIronSoft,		kNoGreen },
	{ kClubIron,	"7 Iron", "ui_7iron", 109, 150, 38,		kSoundSwingIronSoft,		kNoGreen },
	{ kClubIron,	"8 Iron", "ui_8iron", 104, 140, 42,		kSoundSwingIronSoft,		kNoGreen },
	{ kClubIron,	"9 Iron", "ui_9iron", 96, 120, 46.5,	kSoundSwingWedge,			kNoGreen }, 
	{ kClubWedge,	"PW", "ui_pw", 83, 90, 50,				kSoundSwingWedge,			kNoGreen },
	{ kClubWedge,	"SW", "ui_sw", 68, 60, 56,				kSoundSwingWedgeInSand,		kNoGreen },
	{ kClubPutter,	"Putter", "ui_putter", 60, 20, 1,		kSoundSwingPutter,			0 },
	{ kClubPutter,	"Putter", "ui_putter", 75, 40, 5,		kSoundSwingPutter,			kFirePower }
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
			if(club.m_options & kNoRough)
				return false;
			break;
		case kFairwayFringe:
		case kFairway:
			if(club.m_options & kNoFairway)
				return false;
			break;
		case kSandtrap:
			if(club.m_options & kNoSandTrap)
				return false;
			break;
		case kGreenFringe:
		case kGreen:
			if(club.m_options & kNoGreen)
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
	
	// -2: never consider the putters
	
	for(int i = 0; i < kNumGolfClubs - 2; i++)
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


