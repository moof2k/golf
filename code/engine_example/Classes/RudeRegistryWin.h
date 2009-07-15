#ifndef _H_RudeRegistryWin
#define _H_RudeRegistryWin

#include "RudeRegistry.h"

class RudeRegistryWin : public RudeRegistry
{
public:
	RudeRegistryWin(void);
	~RudeRegistryWin(void);

	long QueryByte(TCHAR *app, TCHAR *name, void *buffer, long *buffersize);
	long SetByte(TCHAR *app, TCHAR *name, void *buffer, long buffersize);

private:


};

#endif