#ifndef _H_RudeRegistryWin
#define _H_RudeRegistryWin

#include "RudeRegistry.h"

class RudeRegistryWin : public RudeRegistry
{
public:
	RudeRegistryWin(void);
	~RudeRegistryWin(void);

	int QueryByte(const TCHAR *app, const TCHAR *name, void *buffer, int *buffersize);
	int SetByte(const TCHAR *app, const TCHAR *name, void *buffer, int buffersize);

private:


};

#endif