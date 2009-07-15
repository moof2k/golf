/*
 *  RBKeyMap.cpp
 *  golf
 *
 *  Created by Robert Rose on 10/10/08.
 *  Copyright 2008 Bork 3D LLC. All rights reserved.
 *
 *
 *	An over-done example of how to use the RudeRegistry to save/restore configuration information
 */

#include "RBKeyMap.h"

#include "RudeGlobals.h"
#include "RudeRegistry.h"

#ifdef RUDE_WIN
#include <tchar.h>
#include <windows.h>
#endif


#ifdef RUDE_SYMBIAN
RBKeyMap::RBKeyMap(TUid uid)
: CCoeStatic(uid, EApp)
#else
RBKeyMap::RBKeyMap()
#endif
{
	_vkA = kKeyRotate;
	_vkB = kKeyRotate;
	_vkC = kKeyRotate;
	_vkUp = kKeyUp;
	_vkDown = kKeyDown;
	_vkLeft = kKeyLeft;
	_vkRight = kKeyRight;
	_vkStart = kKeyPause;
	_sound = true;
	
	strcpy(kGDUnknownName, "");
	strcpy(kGDAName , "A");
	strcpy(kGDBName , "B");
	strcpy(kGDCName , "C");
	strcpy(kGDStartName , "START");
	strcpy(kGDUpName , "UP");
	strcpy(kGDDownName , "DOWN");
	strcpy(kGDLeftName , "LEFT");
	strcpy(kGDRightName, "RIGHT");
	
	strcpy(kGD0Name, "0");
	strcpy(kGD1Name, "1");
	strcpy(kGD2Name, "2");
	strcpy(kGD3Name, "3");
	strcpy(kGD4Name, "4");
	strcpy(kGD5Name, "5");
	strcpy(kGD6Name, "6");
	strcpy(kGD7Name, "7");
	strcpy(kGD8Name, "8");
	strcpy(kGD9Name, "9");

	strncpy(_highscores, "MAR150000, IA90000,HED80000,ABR70000,L7L60000,CLM50000,BRW40000,JAD30000,ZAK20000,GFM10000", kHighscoreLen);

	loadKeyMap();

}

RBKeyMap::~RBKeyMap(void)
{
}


#ifdef RUDE_SYMBIAN
RBKeyMap * RBKeyMap::getInstance()
{
	CCoeEnv *coe = CCoeEnv::Static();
	return (RBKeyMap *) coe->Static(kRBKeyMapUid);
}
#else
RBKeyMap *instance = 0;

RBKeyMap * RBKeyMap::getInstance()
{
	if(instance == 0)
		instance = new RBKeyMap();
	return instance;
}
#endif

void RBKeyMap::loadKeyMap()
{
	int buffer;
	int buffersize;
	int result;


	RudeRegistry *reg = RudeRegistry::GetSingleton();
	if(reg == NULL)
		return;

	// LOAD BOOLEANS

	buffersize = sizeof(buffer);


	result = reg->QueryByte(_T("Rude Blocks"), _T("SOUND"), &buffer, &buffersize);
	if(result != 0)
		return;
	buffer &= 0xFF;
	_sound = (bool) buffer;


	// LOAD RPGKeys

	
	result = reg->QueryByte(_T("Rude Blocks"), _T("0"), &buffer, &buffersize);
	if(result != 0)
		return;
	unassignKey((RBKey) buffer);
	_vk0 = (RBKey) buffer;
	
	result = reg->QueryByte(_T("Rude Blocks"), _T("1"), &buffer, &buffersize);
	if(result != 0)
		return;
	unassignKey((RBKey) buffer);
	_vk1 = (RBKey) buffer;
	
	result = reg->QueryByte(_T("Rude Blocks"), _T("2"), &buffer, &buffersize);
	if(result != 0)
		return;
	unassignKey((RBKey) buffer);
	_vk2 = (RBKey) buffer;
	
	result = reg->QueryByte(_T("Rude Blocks"), _T("3"), &buffer, &buffersize);
	if(result != 0)
		return;
	unassignKey((RBKey) buffer);
	_vk3 = (RBKey) buffer;
	
	result = reg->QueryByte(_T("Rude Blocks"), _T("4"), &buffer, &buffersize);
	if(result != 0)
		return;
	unassignKey((RBKey) buffer);
	_vk4 = (RBKey) buffer;
	
	result = reg->QueryByte(_T("Rude Blocks"), _T("5"), &buffer, &buffersize);
	if(result != 0)
		return;
	unassignKey((RBKey) buffer);
	_vk5 = (RBKey) buffer;
	
	result = reg->QueryByte(_T("Rude Blocks"), _T("6"), &buffer, &buffersize);
	if(result != 0)
		return;
	unassignKey((RBKey) buffer);
	_vk6 = (RBKey) buffer;
	
	result = reg->QueryByte(_T("Rude Blocks"), _T("7"), &buffer, &buffersize);
	if(result != 0)
		return;
	unassignKey((RBKey) buffer);
	_vk7 = (RBKey) buffer;
	
	result = reg->QueryByte(_T("Rude Blocks"), _T("8"), &buffer, &buffersize);
	if(result != 0)
		return;
	unassignKey((RBKey) buffer);
	_vk8 = (RBKey) buffer;
	
	result = reg->QueryByte(_T("Rude Blocks"), _T("9"), &buffer, &buffersize);
	if(result != 0)
		return;
	unassignKey((RBKey) buffer);
	_vk9 = (RBKey) buffer;

	result = reg->QueryByte(_T("Rude Blocks"), _T("A"), &buffer, &buffersize);
	if(result != 0)
		return;
	unassignKey((RBKey) buffer);
	_vkA = (RBKey) buffer;

	result = reg->QueryByte(_T("Rude Blocks"), _T("B"), &buffer, &buffersize);
	if(result != 0)
		return;
	unassignKey((RBKey) buffer);
	_vkB = (RBKey) buffer;

	result = reg->QueryByte(_T("Rude Blocks"), _T("C"), &buffer, &buffersize);
	if(result != 0)
		return;
	unassignKey((RBKey) buffer);
	_vkC = (RBKey) buffer;

	result = reg->QueryByte(_T("Rude Blocks"), _T("START"), &buffer, &buffersize);
	if(result != 0)
		return;
	unassignKey((RBKey) buffer);
	_vkStart = (RBKey) buffer;

	result = reg->QueryByte(_T("Rude Blocks"), _T("UP"), &buffer, &buffersize);
	if(result != 0)
		return;
	unassignKey((RBKey) buffer);
	_vkUp = (RBKey) buffer;

	result = reg->QueryByte(_T("Rude Blocks"), _T("DOWN"), &buffer, &buffersize);
	if(result != 0)
		return;
	unassignKey((RBKey) buffer);
	_vkDown = (RBKey) buffer;

	result = reg->QueryByte(_T("Rude Blocks"), _T("LEFT"), &buffer, &buffersize);
	if(result != 0)
		return;
	unassignKey((RBKey) buffer);
	_vkLeft = (RBKey) buffer;

	result = reg->QueryByte(_T("Rude Blocks"), _T("RIGHT"), &buffer, &buffersize);
	if(result != 0)
		return;
	unassignKey((RBKey) buffer);
	_vkRight = (RBKey) buffer;

	// LOAD HIGHSCORES

	buffersize = sizeof(_highscores);

	result = reg->QueryByte(_T("Rude Blocks"), _T("HIGHSCORES"), &_highscores, &buffersize);
	if(result != 0)
		return;



	delete reg;
}

void RBKeyMap::saveKeyMap()
{

	long buffer;
	long buffersize = sizeof(buffer);
	long result;

	RudeRegistry *reg = RudeRegistry::GetSingleton();
	if(reg == NULL)
		return;



	buffer = _sound;
	result = reg->SetByte(_T("Rude Blocks"), _T("SOUND"), &buffer, buffersize);
	if(result != 0)
		return;

	
	buffer = _vk0;
	result = reg->SetByte(_T("Rude Blocks"), _T("0"), &buffer, buffersize);
	if(result != 0)
		return;
		
	buffer = _vk1;
	result = reg->SetByte(_T("Rude Blocks"), _T("1"), &buffer, buffersize);
	if(result != 0)
		return;
	
	buffer = _vk2;
	result = reg->SetByte(_T("Rude Blocks"), _T("2"), &buffer, buffersize);
	if(result != 0)
		return;
		
	buffer = _vk3;
	result = reg->SetByte(_T("Rude Blocks"), _T("3"), &buffer, buffersize);
	if(result != 0)
		return;
		
	buffer = _vk4;
	result = reg->SetByte(_T("Rude Blocks"), _T("4"), &buffer, buffersize);
	if(result != 0)
		return;
		
	buffer = _vk5;
	result = reg->SetByte(_T("Rude Blocks"), _T("5"), &buffer, buffersize);
	if(result != 0)
		return;
		
	buffer = _vk6;
	result = reg->SetByte(_T("Rude Blocks"), _T("6"), &buffer, buffersize);
	if(result != 0)
		return;
		
	buffer = _vk7;
	result = reg->SetByte(_T("Rude Blocks"), _T("7"), &buffer, buffersize);
	if(result != 0)
		return;
		
	buffer = _vk8;
	result = reg->SetByte(_T("Rude Blocks"), _T("8"), &buffer, buffersize);
	if(result != 0)
		return;
		
	buffer = _vk9;
	result = reg->SetByte(_T("Rude Blocks"), _T("9"), &buffer, buffersize);
	if(result != 0)
		return;

	buffer = _vkA;
	result = reg->SetByte(_T("Rude Blocks"), _T("A"), &buffer, buffersize);
	if(result != 0)
		return;

	buffer = _vkB;
	result = reg->SetByte(_T("Rude Blocks"), _T("B"), &buffer, buffersize);
	if(result != 0)
		return;

	buffer = _vkC;
	result = reg->SetByte(_T("Rude Blocks"), _T("C"), &buffer, buffersize);
	if(result != 0)
		return;

	buffer = _vkStart;
	result = reg->SetByte(_T("Rude Blocks"), _T("START"), &buffer, buffersize);
	if(result != 0)
		return;

	buffer = _vkUp;
	result = reg->SetByte(_T("Rude Blocks"), _T("UP"), &buffer, buffersize);
	if(result != 0)
		return;

	buffer = _vkDown;
	result = reg->SetByte(_T("Rude Blocks"), _T("DOWN"), &buffer, buffersize);
	if(result != 0)
		return;

	buffer = _vkLeft;
	result = reg->SetByte(_T("Rude Blocks"), _T("LEFT"), &buffer, buffersize);
	if(result != 0)
		return;

	buffer = _vkRight;
	result = reg->SetByte(_T("Rude Blocks"), _T("RIGHT"), &buffer, buffersize);
	if(result != 0)
		return;

	buffersize = sizeof(_highscores);
	result = reg->SetByte(_T("Rude Blocks"), _T("HIGHSCORES"), &_highscores, buffersize);
	if(result != 0)
		return;
		

}

void RBKeyMap::unassignKey(RBKey keyname)
{
	if(keyname == _vkA)
		_vkA = kKeyUnknown;
	if(keyname == _vkB)
		_vkB = kKeyUnknown;
	if(keyname == _vkC)
		_vkC = kKeyUnknown;
	if(keyname == _vkStart)
		_vkStart = kKeyUnknown;
	if(keyname == _vkUp)
		_vkUp = kKeyUnknown;
	if(keyname == _vkDown)
		_vkDown = kKeyUnknown;
	if(keyname == _vkLeft)
		_vkLeft = kKeyUnknown;
	if(keyname == _vkRight)
		_vkRight = kKeyUnknown;
	
	if(keyname == _vk0)
		_vk0 = kKeyUnknown;
	if(keyname == _vk1)
		_vk1 = kKeyUnknown;
	if(keyname == _vk2)
		_vk2 = kKeyUnknown;
	if(keyname == _vk3)
		_vk3 = kKeyUnknown;
	if(keyname == _vk4)
		_vk4 = kKeyUnknown;
	if(keyname == _vk5)
		_vk5 = kKeyUnknown;
	if(keyname == _vk6)
		_vk6 = kKeyUnknown;
	if(keyname == _vk7)
		_vk7 = kKeyUnknown;
	if(keyname == _vk8)
		_vk8 = kKeyUnknown;
	if(keyname == _vk9)
		_vk9 = kKeyUnknown;
	
}

bool RBKeyMap::setKey(RudeKey k, RBKey keyname)
{
	// is this key even valid?
//	if(!validKey(dwKey, keylist))
//		return false;

	// unassign the previous key mapped to keyname
	unassignKey(keyname);

	// map the new key
	if(k == kRudeKeyA)
		_vkA = keyname;
	else if(k == kRudeKeyB)
		_vkB = keyname;
	else if(k == kRudeKeyC)
		_vkC = keyname;
	else if(k == kRudeKeyStart)
		_vkStart = keyname;
	else if(k == kRudeKeyUp)
		_vkUp = keyname;
	else if(k == kRudeKeyDown)
		_vkDown = keyname;
	else if(k == kRudeKeyLeft)
		_vkLeft = keyname;
	else if(k == kRudeKeyRight)
		_vkRight = keyname;
		
	else if(k == kRudeKey0)
		_vk0 = keyname;
	else if(k == kRudeKey1)
		_vk1 = keyname;
	else if(k == kRudeKey2)
		_vk2 = keyname;
	else if(k == kRudeKey3)
		_vk3 = keyname;
	else if(k == kRudeKey4)
		_vk4 = keyname;
	else if(k == kRudeKey5)
		_vk5 = keyname;
	else if(k == kRudeKey6)
		_vk6 = keyname;
	else if(k == kRudeKey7)
		_vk7 = keyname;
	else if(k == kRudeKey8)
		_vk8 = keyname;
	else if(k == kRudeKey9)
		_vk9 = keyname;
	
	else
		return false;

	return true;
}

RudeKey RBKeyMap::getKey(RBKey keyname)
{
	if(keyname == _vkA)
		return kRudeKeyA;
	if(keyname == _vkB)
		return kRudeKeyB;
	if(keyname == _vkC)
		return kRudeKeyC;
	if(keyname == _vkStart)
		return kRudeKeyStart;
	if(keyname == _vkUp)
		return kRudeKeyUp;
	if(keyname == _vkDown)
		return kRudeKeyDown;
	if(keyname == _vkLeft)
		return kRudeKeyLeft;
	if(keyname == _vkRight)
		return kRudeKeyRight;
		
	if(keyname == _vk0)
		return kRudeKey0;
	if(keyname == _vk1)
		return kRudeKey1;
	if(keyname == _vk2)
		return kRudeKey2;
	if(keyname == _vk3)
		return kRudeKey3;
	if(keyname == _vk4)
		return kRudeKey4;
	if(keyname == _vk5)
		return kRudeKey5;
	if(keyname == _vk6)
		return kRudeKey6;
	if(keyname == _vk7)
		return kRudeKey7;
	if(keyname == _vk8)
		return kRudeKey8;
	if(keyname == _vk9)
		return kRudeKey9;

	return kRudeKeyUnknown;
}

const char * RBKeyMap::getRudeKeyName(RudeKey rudekey)
{
	if(rudekey == kRudeKeyA)
		return kGDAName;
	if(rudekey == kRudeKeyB)
		return kGDBName;
	if(rudekey == kRudeKeyC)
		return kGDCName;
	if(rudekey == kRudeKeyStart)
		return kGDStartName;
	if(rudekey == kRudeKeyUp)
		return kGDUpName;
	if(rudekey == kRudeKeyDown)
		return kGDDownName;
	if(rudekey == kRudeKeyLeft)
		return kGDLeftName;
	if(rudekey == kRudeKeyRight)
		return kGDRightName;
		
	if(rudekey == kRudeKey0)
		return kGD0Name;
	if(rudekey == kRudeKey1)
		return kGD1Name;
	if(rudekey == kRudeKey2)
		return kGD2Name;
	if(rudekey == kRudeKey3)
		return kGD3Name;
	if(rudekey == kRudeKey4)
		return kGD4Name;
	if(rudekey == kRudeKey5)
		return kGD5Name;
	if(rudekey == kRudeKey6)
		return kGD6Name;
	if(rudekey == kRudeKey7)
		return kGD7Name;
	if(rudekey == kRudeKey8)
		return kGD8Name;
	if(rudekey == kRudeKey9)
		return kGD9Name;

	return kGDUnknownName;
}

RBKey RBKeyMap::mapKey(RudeKey k)
{
	if(k == kRudeKeyA)
		return _vkA;
	if(k == kRudeKeyB)
		return _vkB;
	if(k == kRudeKeyC)
		return _vkC;
	if(k == kRudeKeyStart)
		return _vkStart;
	if(k == kRudeKeyUp)
		return _vkUp;
	if(k == kRudeKeyDown)
		return _vkDown;
	if(k == kRudeKeyLeft)
		return _vkLeft;
	if(k == kRudeKeyRight)
		return _vkRight;

	if(k == kRudeKey0)
		return _vk0;
	if(k == kRudeKey1)
		return _vk1;
	if(k == kRudeKey2)
		return _vk2;
	if(k == kRudeKey3)
		return _vk3;
	if(k == kRudeKey4)
		return _vk4;
	if(k == kRudeKey5)
		return _vk5;
	if(k == kRudeKey6)
		return _vk6;
	if(k == kRudeKey7)
		return _vk7;
	if(k == kRudeKey8)
		return _vk8;
	if(k == kRudeKey9)
		return _vk9;
		
	
	return kKeyUnknown;
}


void RBKeyMap::highscores(char *highscores)
{
	strncpy(_highscores, highscores, kHighscoreLen);
}


