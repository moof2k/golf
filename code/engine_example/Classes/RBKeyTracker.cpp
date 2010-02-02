/*
 *  RBKeyTracker.cpp
 *  golf
 *
 *  Created by Robert Rose on 10/10/08.
 *  Copyright 2008 Bork 3D LLC. All rights reserved.
 *
 *
 */

#include "RBkeytracker.h"


RBKeyTracker::RBKeyTracker(void)
{
	init();
}

RBKeyTracker::~RBKeyTracker(void)
{
}



void RBKeyTracker::init()
{
	_keytimer = 0.0f;
	_timermax = 0.10f;

	unsigned int i;

	for(i = 0; i < kKTMaxKeys; i++)
	{
		_keysdown[i] = kKeyUnknown;
	}
}

bool RBKeyTracker::timer(float delta)
{
	_keytimer += delta;

	if(_keytimer > _timermax)
	{
		_keytimer = 0.0f;
		return true;
	}

	return false;
}

void RBKeyTracker::keyDown(RBKey k)
{
	unsigned int i;

	keyUp(k);

	for(i = 0; i < kKTMaxKeys; i++)
	{
		if(_keysdown[i] == kKeyUnknown)
		{
			_keysdown[i] = k;
			_keytimer = 0.0f;
			return;
		}
	}
}

void RBKeyTracker::keyUp(RBKey k)
{
	unsigned int i;

	for(i = 0; i < kKTMaxKeys; i++)
	{
		if(_keysdown[i] == k)
			_keysdown[i] = kKeyUnknown;
	}
}

bool RBKeyTracker::isDown(RBKey k)
{
	int i;

	for(i = 0; i < kKTMaxKeys; i++)
	{
		if(_keysdown[i] == k)
			return true;
	}

	return false;
}

void RBKeyTracker::getKeyStr(TCHAR *str)
{
	unsigned int i;

	_stprintf(str, _T("  "));

	for(i = 0; i < kKTMaxKeys; i++)
	{
		switch(_keysdown[i])
		{
			case kKeyLeft:
				_tcscat(str, _T("l "));
				break;
			case kKeyRight:
				_tcscat(str, _T("r "));
				break;
			case kKeyDown:
				_tcscat(str, _T("d "));
				break;
			case kKeyRotate:
				_tcscat(str, _T("o "));
				break;
			case kKeyPause:
				_tcscat(str, _T("p "));
				break;
			case kKeyUnknown:
			default:
				_tcscat(str, _T("u "));
				break;
		}
	}
}
