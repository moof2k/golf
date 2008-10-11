/*
 *  RBGolfClub.cpp
 *  golf
 *
 *  Created by Robert Rose on 9/17/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "RBGolfClub.h"
#include "Rude.h"
#include "RudeSound.h"


// http://www.golfspyder.com/golf-club-loft.html

static RBGolfClub sGolfClubs[kNumGolfClubs] = {
	{ "1 Wood", "1wood", 200, 240, 11, kSoundSwingDriver },
	{ "3 Wood", "3wood", 180, 225, 14, kSoundSwingWood },
	{ "5 Wood", "5wood", 160, 200, 21, kSoundSwingWood },
	{ "3 Iron", "3iron", 140, 190, 22, kSoundSwingIronSoft },
	{ "4 Iron", "4iron", 130, 175, 25.5, kSoundSwingIronSoft },
	{ "5 Iron", "5iron", 120, 160, 30, kSoundSwingIronSoft },
	{ "7 Iron", "7iron", 120, 150, 38, kSoundSwingIronSoft },
	{ "8 Iron", "8iron", 120, 140, 42, kSoundSwingIronSoft },
	{ "9 Iron", "9iron", 120, 120, 46.5, kSoundSwingWedge }, 
	{ "PW", "pw", 90, 90, 50, kSoundSwingWedge },
	{ "SW", "sw", 70, 60, 56, kSoundSwingWedgeInSand },
	{ "Putter", "putter", 30, 30, 1, kSoundSwingPutter }
};


RBGolfClub * RBGolfClub::GetClub(int num)
{
	RUDE_ASSERT(num < kNumGolfClubs, "Invalid club");
	
	return &sGolfClubs[num];
}

int RBGolfClub::NextClub(int n)
{
	n++;
	
	if(n >= kNumGolfClubs)
		n = 0;
	
	return n;
}

int RBGolfClub::PrevClub(int n)
{
	n--;
	
	if(n < 0)
		n = kNumGolfClubs - 1;
	
	return n;	
}


