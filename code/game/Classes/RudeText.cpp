/*
 *  RudeText.cpp
 *  golf
 *
 *  Created by Robert Rose on 9/3/08.
 *  Copyright 2008 Bork 3D LLC. All rights reserved.
 *
 */

#include "RudeText.h"

#include <stdarg.h>

#include "PVRTFixedPoint.h"
#include "PVRTPrint3D.h"


CPVRTPrint3D gTextPrint;

void RudeText::Init()
{
	gTextPrint.SetTextures(0, 480, 320);
}

void RudeText::Print(float x, float y, float scale, unsigned int color, const char * const pszFormat, ...)
{
	va_list args;
	char text[512];
	
	va_start(args, pszFormat);
	vsprintf(text, pszFormat, args);
	va_end(args);
	
	gTextPrint.Print3D(x, y, scale, color, text);
}

void RudeText::Flush()
{
	gTextPrint.Flush();
}