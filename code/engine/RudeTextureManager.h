/*
 *  RudeTextureManager.h
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef __H_RudeTextureManager
#define __H_RudeTextureManager

#include "Rude.h"
#include "RudeTexture.h"

#include <vector>

class RudeTextureManager {
	
public:
	RudeTextureManager();
	~RudeTextureManager();
	
	static RudeTextureManager * GetInstance();
	
	int LoadTextureFromPVRTFile(const char *name);
	int LoadTextureFromPVRTPointer(const char *name, const void *data);
	int LoadTextureFromPNGFile(const char *name);
	
	void ReleaseTexture(int texid);
	
	int ReplaceTextureFromPNGFile(int texid, const char *name);
	
	RudeTexture * GetTexture(int id)
	{
		RUDE_ASSERT(id >= 0, "Invalid id");
		RUDE_ASSERT(((unsigned int) id) < m_textures.size(), "Invalid id");
		RUDE_ASSERT(m_textures[id], "Invalid texture");
		
		return m_textures[id];
	}
	
	int GetTextureID(const char *name);
	
	void SetTexture(int id)
	{
		RudeTexture *texture = GetTexture(id);
		texture->SetActive();
	}
	
private:
	
	int InsertTexture(RudeTexture *texture);
	
	std::vector<RudeTexture *> m_textures;
	
};


#endif
