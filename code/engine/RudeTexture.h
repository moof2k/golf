/*
 *  RudeTexture.h
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef __H_RudeTexture
#define __H_RudeTexture

const int kNameLen = 32;

class RudeTexture {

public:
	RudeTexture();
	~RudeTexture();
	
	int LoadFromPVRTFile(const char *name);
	int LoadFromPVRTPointer(const char *name, const void *data);
	int LoadFromPNG(const char *name);

	void SetActive();
	
	const char * GetName() { return m_name; }
	
	int GetWidth() { return m_width; }
	int GetHeight() { return m_height; }
	
private:
	
	char m_name[kNameLen];
	unsigned int m_texture;
	int m_height;
	int m_width;
	
};

#endif


