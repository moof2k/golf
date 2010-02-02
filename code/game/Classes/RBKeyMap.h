/*
 *  RBKeyMap.h
 *  golf
 *
 *  Created by Robert Rose on 10/10/08.
 *  Copyright 2008 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef _H_RPGKeyMap
#define _H_RPGKeyMap

#include "RudeGlobals.h"
#include "RudeKey.h"

#ifdef RUDE_SYMBIAN
#include <coemain.h>
#endif

typedef enum {
	kKeyUnknown = 0,
	kKeyLeft,
	kKeyRight,
	kKeyPause,
	kKeyRotate,
	kKeyUp,
	kKeyDown 
} RBKey;

#define kHighscoreLen 140

#ifdef RUDE_SYMBIAN
static const TUid kRBKeyMapUid = { 0x01f4fffc };
class RBKeyMap : public CCoeStatic
#else
class RBKeyMap
#endif
{
public:
#ifdef RUDE_SYMBIAN
	RBKeyMap(TUid uid);
#else
	RBKeyMap();
#endif
	~RBKeyMap(void);


	static RBKeyMap *getInstance();

	RBKey mapKey(RudeKey k);

	bool setKey(RudeKey k, RBKey keyname);
	void unassignKey(RBKey keyname);
	
	RudeKey getKey(RBKey keyname);
	const char * getRudeKeyName(RudeKey rudekey);
	const char * getKeyName(RBKey key) { return getRudeKeyName(getKey(key)); }

	bool sound() { return _sound; }
	void sound(bool s) { _sound = s; }


	void highscores(char *highscores);
	char * highscores() { return _highscores; }

	void loadKeyMap();
	void saveKeyMap();

private:

	bool _sound;
	char _highscores[kHighscoreLen];


	RBKey _vkA;
	RBKey _vkB;
	RBKey _vkC;
	RBKey _vkUp;
	RBKey _vkDown;
	RBKey _vkLeft;
	RBKey _vkRight;
	RBKey _vkStart;
	
	RBKey _vk0;
	RBKey _vk1;
	RBKey _vk2;
	RBKey _vk3;
	RBKey _vk4;
	RBKey _vk5;
	RBKey _vk6;
	RBKey _vk7;
	RBKey _vk8;
	RBKey _vk9;
	
	char kGDUnknownName[7];
	char kGDAName[7];
	char kGDBName[7];
	char kGDCName[7];
	char kGDStartName[7];
	char kGDUpName[7];
	char kGDDownName[7];
	char kGDLeftName[7];
	char kGDRightName[7];
	char kGD0Name[4];
	char kGD1Name[4];
	char kGD2Name[4];
	char kGD3Name[4];
	char kGD4Name[4];
	char kGD5Name[4];
	char kGD6Name[4];
	char kGD7Name[4];
	char kGD8Name[4];
	char kGD9Name[4];

};

#endif
