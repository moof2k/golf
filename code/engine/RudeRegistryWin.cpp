#include "ruderegistrywin.h"

#include <WinError.h>

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

	_stprintf(keypath, _T("SOFTWARE\\Bork3D\\%s"), app);

	LONG result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, keypath,
		NULL, KEY_ALL_ACCESS | KEY_WOW64_32KEY, &hkey);
	if(result != ERROR_SUCCESS)
		return result;

	result = RegQueryValueEx(hkey, name, NULL, &type, (LPBYTE) buffer, &bufsize);
	
	*buffersize = bufsize;
	RegCloseKey(hkey);

	return result;
}

int RudeRegistryWin::SetByte(const TCHAR *app, const TCHAR *name, void *buffer, int buffersize)
{
	HKEY bork3d_hkey, app_hkey;

	DWORD disposition;
	LONG result;

	result = RegCreateKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Bork3D"),
		NULL, NULL, NULL, KEY_ALL_ACCESS | KEY_WOW64_32KEY, NULL, &bork3d_hkey, &disposition);
	RUDE_ASSERT(result == ERROR_SUCCESS, "Unable to open HKEY_LOCAL_MACHINE\\SOFTWARE\\Bork3D - %d", result);

	result = RegCreateKeyEx(bork3d_hkey, app,
		NULL, NULL, NULL, KEY_ALL_ACCESS | KEY_WOW64_32KEY, NULL, &app_hkey, &disposition);
	RUDE_ASSERT(result == ERROR_SUCCESS, "Unable to open HKEY_LOCAL_MACHINE\\SOFTWARE\\Bork3D\\%s - %d", app, result);


	result = RegSetValueEx(app_hkey, name, NULL, REG_BINARY, (LPBYTE) buffer, buffersize);
	RUDE_ASSERT(result == ERROR_SUCCESS, "Unable to write registry key, got error %d", result);
	
	RegCloseKey(app_hkey);
	RegCloseKey(bork3d_hkey);

	return 0;
}
