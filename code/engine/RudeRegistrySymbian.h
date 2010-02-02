#ifndef _H_RudeRegistrySymbian
#define _H_RudeRegistrySymbian

#include "RudeRegistry.h"

class RudeRegistrySymbian : public RudeRegistry
{
public:
	RudeRegistrySymbian(void);
	~RudeRegistrySymbian(void);

	long QueryByte(TCHAR *app, TCHAR *name, void *buffer, long *buffersize);
	long SetByte(TCHAR *app, TCHAR *name, void *buffer, long buffersize);

private:


};

#endif