#ifndef _H_RudeRegistry
#define _H_RudeRegistry

#include "RudeGlobals.h"

class RudeRegistry
{
public:
	RudeRegistry(void);
	virtual ~RudeRegistry(void);

	static RudeRegistry * GetSingleton();

	virtual int QueryByte(TCHAR *app, TCHAR *name, void *buffer, int *buffersize) = 0;
	virtual int SetByte(TCHAR *app, TCHAR *name, void *buffer, int buffersize) = 0;

};

#endif