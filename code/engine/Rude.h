#ifndef _H_Rude
#define _H_Rude


#define RUDE_DEBUG

// DETERMINE PLATFORM ------------------------------------------------------

// Windows Desktop
#ifdef _WINDOWS
#ifndef __MWERKS__
#define RUDE_GAPI
#define RUDE_WIN
#define RUDE_WIN_DESK
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#endif
#endif

// Pocket PC / SmartPhone
#ifdef _WIN32_WCE
#define RUDE_GAPI
#define RUDE_WIN
#define RUDE_WIN_SP
#endif

// Palm OS (these are probably poor assumptions, but work)
#ifdef __MWERKS__
#ifdef __MC68K__
#define RUDE_PALM
#endif
#endif

#ifdef __SYMBIAN32__
#define RUDE_SYMBIAN
#define RUDE_GAPI
#endif

#if defined(__GNUC__) && ( defined(__APPLE_CPP__) || defined(__APPLE_CC__) || defined(__MACOS_CLASSIC__) )
#include <TargetConditionals.h>
#if defined(TARGET_OS_IPHONE) || defined(TARGET_IPHONE_SIMULATOR)
#define RUDE_IPHONE

#endif
#endif

// -------------------------------------------------------------------------

#ifdef RUDE_GAPI

#ifdef WIN32_PLATFORM_WFSP
#define RUDE_NOSTYLUS
#endif

#endif

#ifdef RUDE_PALM
#include <StdIOPalm.h>
#include <StringMgr.h>

#define free MemPtrFree
#define malloc MemPtrNew
#define memset MemSet
#define memcpy MemMove

#define TCHAR char
#define _T(a) a

#define strcpy StrCopy
#define strcat StrCat
#define strncpy StrNCopy
#define strncat StrNCat
#define strlen StrLen
#define sprintf StrPrintF

#define _stprintf sprintf
#define snprintf(s, l, f) sprintf(s, f)
#define _snprintf(s, l, f, a) sprintf(s, f, a)
#define _tcscat strcat

#endif

#ifdef RUDE_SYMBIAN

#define RUDE_NOSTYLUS

#include <coemain.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef RUDE_GAPI
#include "GapiDrawDefines.h"
#else
#define TCHAR char
#define _stprintf sprintf
#define _tcscat strcat
#define _tcscpy strcpy
#define _tcslen strlen
#endif

#define _T(a) a

#define KUidVectorBlaster 0x1020153F

#endif







#ifdef RUDE_IPHONE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TCHAR char
#define _stprintf sprintf
#define _tcscat strcat
#define _tcscpy strcpy
#define _tcslen strlen

#define _T(a) a

#endif // RUDE_IPHONE


//  ASSERTS...
#ifndef __RUDE__DEBUG
#define __RUDE__DEBUG
#ifdef RUDE_DEBUG

#include <stdarg.h>

inline void RudeAssert(const char *file, int line, const char *pszFormat, ...)
{
	va_list args;
	char text[512];
	
	va_start(args, pszFormat);
	vsprintf(text, pszFormat, args);
	va_end(args);
	
	printf("ASSERT: %s (%d): %s\n", file, line, text);
	exit(1);
}

#define RUDE_ASSERT(x, ...) if((x) == 0) RudeAssert(__FILE__, __LINE__, __VA_ARGS__)
#else
#define RUDE_ASSERT(x, y) (0)
#endif
#endif



#endif