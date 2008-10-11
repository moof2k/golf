
#include "RudeTextureManager.h"



static RudeTextureManager *gTextureManager = 0;

RudeTextureManager * RudeTextureManager::GetInstance()
{
	if(gTextureManager == 0)
		gTextureManager = new RudeTextureManager();
	
	return gTextureManager;
}


RudeTextureManager::RudeTextureManager()
{
}

RudeTextureManager::~RudeTextureManager()
{
}

int RudeTextureManager::GetTextureID(const char *name)
{
	int s = m_textures.size();
	
	for(int i = 0; i < s; i++)
	{
		const char *texname = m_textures[i]->GetName();
		
		if(strcmp(name, texname) == 0)
			return i;
	}
	
	return -1;
}


int RudeTextureManager::LoadTextureFromPVRTFile(const char *name)
{
	// check to make sure its not already loaded
	int texid = GetTextureID(name);
	if(texid >= 0)
		return texid;
	
	RudeTexture *tex = new RudeTexture();
	
	int result = tex->LoadFromPVRTFile(name);
	
	RUDE_ASSERT(result >= 0, "Unable to load texture");
	
	if(result < 0)
	{
		delete tex;
		return result;
	}
	
	m_textures.push_back(tex);
	
	return m_textures.size() - 1;
}


int RudeTextureManager::LoadTextureFromPVRTPointer(const char *name, const void *data)
{
	// check to make sure its not already loaded
	int texid = GetTextureID(name);
	if(texid >= 0)
		return texid;
	
	RudeTexture *tex = new RudeTexture();
	
	int result = tex->LoadFromPVRTPointer(name, data);
	
	RUDE_ASSERT(result >= 0, "Unable to load texture");
	
	if(result < 0)
	{
		delete tex;
		return result;
	}
	
	m_textures.push_back(tex);
	
	return m_textures.size() - 1;
}


int RudeTextureManager::LoadTextureFromPNGFile(const char *name)
{
	// check to make sure its not already loaded
	int texid = GetTextureID(name);
	if(texid >= 0)
		return texid;
	
	RudeTexture *tex = new RudeTexture();
	
	int result = tex->LoadFromPNG(name);
	
	RUDE_ASSERT(result >= 0, "Unable to load texture");
	
	if(result < 0)
	{
		delete tex;
		return result;
	}
	
	m_textures.push_back(tex);
	
	return m_textures.size() - 1;
}


