/*
 *  RudeDebug.h
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef __H_RudeDebug
#define __H_RudeDebug

#define kRudeDebugTextLen 5
#define kRudeDebugTextSize 80

class RudeDebug
{
public:
	static void Init();
	
	static void Write(const char *file, int line, const char *format, ...);

	static void PurgeLog();

private:

	static char sLogfile[80];
	
};

#define RUDE_REPORT(...)  RudeDebug::Write(__FILE__, __LINE__, __VA_ARGS__)


#endif

