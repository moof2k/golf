#include "RudeRegistrySymbian.h"
#include "RudeDebug.h"

#include <eikenv.h>


RudeRegistrySymbian::RudeRegistrySymbian(void)
{
}

RudeRegistrySymbian::~RudeRegistrySymbian(void)
{
}


long RudeRegistrySymbian::QueryByte(TCHAR *app, TCHAR *name, void *buffer, long *buffersize)
{


	return 0;
}

long RudeRegistrySymbian::SetByte(TCHAR *app, TCHAR *name, void *buffer, long buffersize)
{
	TInt err;
	RudeDebug *debug = RudeDebug::GetInstance();
	
	_LIT(KMyFile,"\\system\\Apps\\VectorBlaster\\file.txt");
	RFile myFile;
	
	
	err = myFile.Open(CEikonEnv::Static()->FsSession(), KMyFile, EFileWrite);
	
	debug->Print(_T("myFile.Open"), err);
	
	if(err == KErrNotFound)
	{
	    err = myFile.Create(CEikonEnv::Static()->FsSession(), KMyFile, EFileWrite);
	    debug->Print(_T("myFile.Create"), err);
	}
	
	
	if(err != KErrNone)
		return err;
	
	_LIT8(KWriteBuf,"write data");
	myFile.Write(KWriteBuf);
	
	myFile.Close();

	debug->Print(_T("myFile.Close"));
	
	return 0;
}
