#include "ruderegistrycf.h"

#include <CoreFoundation/CFPreferences.h>

RudeRegistryCF::RudeRegistryCF(void)
{
}

RudeRegistryCF::~RudeRegistryCF(void)
{
}


int RudeRegistryCF::QueryByte(TCHAR *app, TCHAR *name, void *buffer, int *buffersize)
{
	CFStringRef key = CFStringCreateWithCString(NULL, name, kCFStringEncodingASCII);
	CFPropertyListRef value = CFPreferencesCopyAppValue(key, kCFPreferencesCurrentApplication);
	
	if(value == 0)
		return -1;
	
	if(CFDataGetLength((CFDataRef) value) != *buffersize)
		return -1;
	
	CFRange range = CFRangeMake(0, *buffersize);
	CFDataGetBytes((CFDataRef) value, range, (UInt8 *) buffer);

	return 0;
}

int RudeRegistryCF::SetByte(TCHAR *app, TCHAR *name, void *buffer, int buffersize)
{
	CFPropertyListRef value = CFDataCreate(NULL, (UInt8 *) buffer, buffersize);
	CFStringRef key = CFStringCreateWithCString(NULL, name, kCFStringEncodingASCII);
	CFPreferencesSetAppValue(key, value, kCFPreferencesCurrentApplication);

	return 0;
}
