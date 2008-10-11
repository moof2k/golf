#ifndef _H_RudeRegistry
#define _H_RudeRegistry

#include "RudeGlobals.h"

class RudeRegistry
{
public:
	RudeRegistry(void);
	virtual ~RudeRegistry(void);

	static RudeRegistry * CreateRegistry();

	virtual long QueryByte(TCHAR *app, TCHAR *name, void *buffer, long *buffersize) = 0;
	virtual long SetByte(TCHAR *app, TCHAR *name, void *buffer, long buffersize) = 0;

};

#endif