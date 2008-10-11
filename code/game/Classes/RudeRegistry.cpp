#include "ruderegistry.h"

#ifdef RUDE_WIN
#include "RudeRegistryWin.h"
#include "RudeRegistryText.h"
#endif

#ifdef RUDE_SYMBIAN
#include "RudeRegistrySymbian.h"
#include "RudeRegistryText.h"
#endif

#ifdef RUDE_IPHONE
#include "RudeRegistryCF.h"
#endif

RudeRegistry::RudeRegistry(void)
{
}

RudeRegistry::~RudeRegistry(void)
{
}

RudeRegistry * RudeRegistry::CreateRegistry()
{
#ifdef RUDE_WIN
	RudeRegistryWin *reg = new RudeRegistryWin();

	return (RudeRegistry *) reg;
#endif

#ifdef RUDE_SYMBIAN
	RudeRegistryText *reg = new RudeRegistryText();
	
	return (RudeRegistry *) reg;
#endif
	
#ifdef RUDE_IPHONE
	RudeRegistryCF *reg = new RudeRegistryCF();
	
	return (RudeRegistry *) reg;
#endif


	return NULL;
}