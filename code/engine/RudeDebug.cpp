/*
 *  RudeDebug.cpp
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#include "RudeDebug.h"
#include "Rude.h"

char RudeDebug::sLogfile[80];


void RudeDebug::Init()
{
#if defined(RUDE_SYMBIAN)
	char path[300];
	RudeGlobals::GetPath(path);
	strcat(path, "log.txt");
	strcpy(sLogfile, path);
#elif defined(RUDE_IPHONE)
	strcpy(sLogfile, "/tmp/rude.log");
#else
	strcpy(sLogfile, "log.txt");
#endif

	//PurgeLog();

}



void RudeDebug::Write(const char *filename, int line, const char *format, ...)
{
	//FILE *file = fopen(sLogfile, "a");
	//if(file == NULL)
	//	return;
	
	char msg[512];
	
	va_list args;
	va_start (args, format);
	vsprintf (msg,format, args);
	va_end (args);

	printf("%s", msg);
	//fprintf(file, "%s (%d): %s", filename, line, msg);
	//fclose(file);

#ifdef RUDE_WIN
	OutputDebugString(msg);
#endif

}

void RudeDebug::PurgeLog()
{
	FILE *file = fopen(sLogfile, "w");
	if(file == NULL)
		return;

	fprintf(file, "begin\n");

	fclose(file);
}



