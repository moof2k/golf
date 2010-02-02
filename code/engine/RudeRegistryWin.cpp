#include "ruderegistrywin.h"

RudeRegistryWin::RudeRegistryWin(void)
{
}

RudeRegistryWin::~RudeRegistryWin(void)
{
}


int RudeRegistryWin::QueryByte(const TCHAR *app, const TCHAR *name, void *buffer, int *buffersize)
{
	HKEY hkey;
	DWORD type;
	DWORD bufsize = *buffersize;
	TCHAR keypath[400];

	_stprintf(keypath, _T("SOFTWARE\\Rude Scotsman\\%s"), app);

	LONG result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, keypath,
						NULL, KEY_ALL_ACCESS, &hkey);
	if(result != ERROR_SUCCESS)
		return result;

	result = RegQueryValueEx(hkey, name, NULL, &type, (LPBYTE) buffer, &bufsize);
	
	*buffersize = bufsize;
	RegCloseKey(hkey);

	return result;
}

int RudeRegistryWin::SetByte(const TCHAR *app, const TCHAR *name, void *buffer, int buffersize)
{
	HKEY hkey;
	DWORD type = REG_BINARY;
	DWORD bufsize = buffersize;
	TCHAR keypath[400];

	_stprintf(keypath, _T("SOFTWARE\\Rude Scotsman\\%s"), app);

	LONG result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, keypath,
						NULL, KEY_ALL_ACCESS, &hkey);
	if(result != ERROR_SUCCESS)
		return result;

	result = RegSetValueEx(hkey, name, NULL, type, (LPBYTE) buffer, bufsize);
	
	RegCloseKey(hkey);

	return result;
}
