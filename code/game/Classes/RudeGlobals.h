#ifndef _H_RudeGlobals
#define _H_RudeGlobals

#include "Rude.h"

#include "RudeColor.h"


class RudeGlobals
{
public:
	RudeGlobals();

	~RudeGlobals();

	static RudeGlobals * GetInstance();


	static void GetPath(char *path);
	

private:

	
};


#endif

