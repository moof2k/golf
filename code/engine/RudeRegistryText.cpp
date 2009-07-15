/*
 *  RudeRegistryText.cpp
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */


#include "RudeRegistryText.h"



RudeRegistryText::RudeRegistryText(void)
{
}

RudeRegistryText::~RudeRegistryText(void)
{
}


long RudeRegistryText::QueryByte(TCHAR *app, TCHAR *name, void *buffer, long *buffersize)
{
	char filename[512];
	
	char path[512];
	RudeGlobals::GetPath(path);
	sprintf(filename, "%s%s%s.dat", path, app, name);


	FILE *file = fopen(filename, "r");
	if(file == NULL)
		return -1;
	
	fread(buffer, *buffersize, 1, file);

	fclose(file);

	return 0;
}

long RudeRegistryText::SetByte(TCHAR *app, TCHAR *name, void *buffer, long buffersize)
{
	char filename[512];
	
	char path[512];
	RudeGlobals::GetPath(path);
	sprintf(filename, "%s%s%s.dat", path, app, name);
	
	FILE *file = fopen(filename, "w");
	if(file == NULL)
		return -1;

	fwrite(buffer, buffersize, 1, file);

	fclose(file);

	return 0;
}
