#ifndef _H_RudeRegistryCF
#define _H_RudeRegistryCF

#include "RudeRegistry.h"

class RudeRegistryCF : public RudeRegistry
{
public:
	RudeRegistryCF(void);
	~RudeRegistryCF(void);

	long QueryByte(TCHAR *app, TCHAR *name, void *buffer, long *buffersize);
	long SetByte(TCHAR *app, TCHAR *name, void *buffer, long buffersize);

private:


};

#endif