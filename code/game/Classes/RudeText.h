/*
 *  RudeText.h
 *  golf
 *
 *  Created by Robert Rose on 9/3/08.
 *  Copyright 2008 Bork 3D LLC. All rights reserved.
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
