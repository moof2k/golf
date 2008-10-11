/******************************************************************************

 @File         PVRTGlobal.h

 @Title        

 @Copyright    Copyright (C) 2007 - 2008 by Imagination Technologies Limited.

 @Platform     ANSI compatible

 @Description  Global defines and typedefs for PVRTools

******************************************************************************/
#ifndef _PVRTGLOBAL_H_
#define _PVRTGLOBAL_H_

/*!***************************************************************************
 Macros
*****************************************************************************/
#ifndef PVRTOOLS_DEBUG_OUTPUT
	#if defined(_DEBUG) || defined(DEBUG)
		#if defined(__SYMBIAN32__)
			#include <e32debug.h>
			#define PVRTOOLS_DEBUG_OUTPUT(A) RDebug::Printf(A);
		#elif defined(_WIN32) && !defined(UNDER_CE)
			#define PVRTOOLS_DEBUG_OUTPUT(A) OutputDebugStringA(A);
		#else
			#define PVRTOOLS_DEBUG_OUTPUT(A) fprintf(stderr,A);
		#endif
	#else
		#define PVRTOOLS_DEBUG_OUTPUT(A)
	#endif
#endif

#define PVRT_MIN(a,b)            (((a) < (b)) ? (a) : (b))
#define PVRT_MAX(a,b)            (((a) > (b)) ? (a) : (b))
#define PVRT_CLAMP(x, l, h)      (PVRT_MIN((h), PVRT_MAX((x), (l))))

#if defined(_WIN32) && !defined(UNDER_CE) && !defined(__SYMBIAN32__)	/* Windows desktop */
	#define _CRTDBG_MAP_ALLOC
	#include <windows.h>
	#include <crtdbg.h>
	#include <tchar.h>
#endif

#if defined(UNDER_CE)
	#include <windows.h>

#ifndef _ASSERT
	#ifdef _DEBUG
		#define _ASSERT(X) { (X) ? 0 : DebugBreak(); }
	#else
		#define _ASSERT(X)
	#endif
#endif

#ifndef _ASSERTE
	#ifdef _DEBUG
		#define _ASSERTE _ASSERT
	#else
		#define _ASSERTE(X)
	#endif
#endif
	#define _RPT0(a,b)
	#define _RPT1(a,b,c)
	#define _RPT2(a,b,c,d)
	#define _RPT3(a,b,c,d,e)
	#define _RPT4(a,b,c,d,e,f)
#else

#ifdef WIN32

#else
#ifdef __linux__
	#define _ASSERT(a)((void)0)
	#define _ASSERTE(a)((void)0)
	#ifdef _DEBUG
		#define _RPT0(a,b) printf(b)
		#define _RPT1(a,b,c) printf(b,c)
	#else
	    #define _RPT0(a,b)((void)0)
	    #define _RPT1(a,b,c)((void)0)
	#endif
	#define _RPT2(a,b,c,d)((void)0)
	#define _RPT3(a,b,c,d,e)((void)0)
	#define _RPT4(a,b,c,d,e,f)((void)0)
	#include <stdlib.h>
	#include <string.h>
	#define BYTE unsigned char
	#define WORD unsigned short
	#define DWORD unsigned long
	typedef struct tagRGBQUAD {
	BYTE    rgbBlue;
	BYTE    rgbGreen;
	BYTE    rgbRed;
	BYTE    rgbReserved;
	} RGBQUAD;
	#define BOOL int
	#define TRUE 1
	#define FALSE 0
#else
	#define _CRT_WARN 0
	#define _RPT0(a,b)
	#define _RPT1(a,b,c)
	#define _RPT2(a,b,c,d)
	#define _RPT3(a,b,c,d,e)
	#define _RPT4(a,b,c,d,e,f)
	#define _ASSERT(X)
	#define _ASSERTE(X)
#endif
#endif
#endif

#include <stdio.h>

#define FREE(X)		{ if(X) { free(X); (X) = 0; } }

// This macro is used to check at compile time that types are of a certain size
// If the size does not equal the expected size, this typedefs an array of size 0 
// which causes a compile error
#define PVRTSIZEASSERT(T, size) typedef int (sizeof_##T)[sizeof(T) == (size)]


namespace PVRT
{
	/****************************************************************************
	** Integer types
	****************************************************************************/
	typedef signed char        int8;
	typedef signed short       int16;
	typedef signed int         int32;
	
	typedef unsigned char      uint8;
	typedef unsigned short     uint16;
	typedef unsigned int       uint32;
	
#if defined(__int64) || defined(_WIN32)
	typedef signed __int64     int64;
	typedef unsigned __int64   uint64;
#elif defined(TInt64)
	typedef TInt64             int64;
	typedef TUInt64            uint64;
#else
	typedef signed long long   int64;
	typedef unsigned long long uint64;
#endif

	PVRTSIZEASSERT(int8,   1);
	PVRTSIZEASSERT(uint8,  1);
	PVRTSIZEASSERT(int16,  2);
	PVRTSIZEASSERT(uint16, 2);
	PVRTSIZEASSERT(int32,  4);
	PVRTSIZEASSERT(uint32, 4);
	PVRTSIZEASSERT(int64,  8);
	PVRTSIZEASSERT(uint64, 8);

	/****************************************************************************
	** swap template function
	****************************************************************************/
	template <typename T>
	void swap(T& a, T& b)
	{
		T temp = a;
		a = b;
		b = temp;
	}

}

#endif // _PVRTGLOBAL_H_

/*****************************************************************************
 End of file (Tools.h)
*****************************************************************************/
