/*
 *  RuideText.h
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef __H_RudeText
#define __H_RudeText

class RudeText
{
public:
	
	static void Init();
	static void Print(float x, float y, float scale, unsigned int color, const char * const pszFormat, ...);
	static void Flush();
	
};


#endif
