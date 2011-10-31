/*
 *  Rude.h
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009-2011 Bork 3D LLC. All rights reserved.
 *
 *	Permission is granted to use this software, in source code or binary form,
 *	with or without modification, for NONCOMMERCIAL PURPOSES provided:
 *	
 *	1) The user of this software (YOU) does not do so in a means intended to
 *	derive monetary compensation or commercial advantage.
 *	
 *	2) Redistributions of the source code contain this license notice, unmodified.
 *	
 *	3) Redistributions in binary form give credit to this software using the
 *	text, "Made with the Bork 3D Game Engine," either in an "About," "Credits,"
 *	or other prominent location in the binary run-time form.
 *	
 *	Commercial users of this software must obtain a Bork 3D Game Engine Commercial
 *	Use License.  See http://bork3d.com for details.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *	THE SOFTWARE.
 */

#ifndef __H_Rude
#define __H_Rude

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

#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>

#include "glext.h"
#include "wglext.h"

extern PFNGLACTIVETEXTUREPROC           glActiveTexture;
extern PFNGLCLIENTACTIVETEXTUREPROC     glClientActiveTexture;
extern PFNGLCOMPRESSEDTEXIMAGE2DPROC    glCompressedTexImage2D;
extern PFNGLCREATEPROGRAMOBJECTARBPROC  glCreateProgramObjectARB;
extern PFNGLDELETEOBJECTARBPROC         glDeleteObjectARB;
extern PFNGLUSEPROGRAMOBJECTARBPROC     glUseProgramObjectARB;
extern PFNGLCREATESHADEROBJECTARBPROC   glCreateShaderObjectARB;
extern PFNGLSHADERSOURCEARBPROC         glShaderSourceARB;
extern PFNGLCOMPILESHADERARBPROC        glCompileShaderARB;
extern PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB;
extern PFNGLATTACHOBJECTARBPROC         glAttachObjectARB;
extern PFNGLGETINFOLOGARBPROC           glGetInfoLogARB;
extern PFNGLLINKPROGRAMARBPROC          glLinkProgramARB;
extern PFNGLGETUNIFORMLOCATIONARBPROC   glGetUniformLocationARB;
extern PFNGLUNIFORM4FARBPROC            glUniform4fARB;
extern PFNGLUNIFORM4FVARBPROC           glUniform4fvARB;
extern PFNGLUNIFORM1IARBPROC            glUniform1iARB;
extern PFNGLUNIFORM1FARBPROC            glUniform1fARB;
extern PFNGLUNIFORMMATRIX4FVARBPROC     glUniformMatrix4fvARB;
extern PFNGLVERTEXATTRIB4FVARBPROC      glVertexAttrib4fvARB;
extern PFNGLBINDATTRIBLOCATIONARBPROC   glBindAttribLocationARB;
extern PFNGLGETACTIVEATTRIBARBPROC      glGetActiveAttribARB;
extern PFNGLGETATTRIBLOCATIONARBPROC    glGetAttribLocationARB;

typedef unsigned long long u64;
typedef int GLfixed;
typedef unsigned int GLuint;
typedef float GLfloat;

#define snprintf _snprintf

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

// Symbian
#ifdef __SYMBIAN32__
#define RUDE_SYMBIAN
#define RUDE_GAPI
#endif

// iPhone / MacOS
#if defined(__GNUC__) && ( defined(__APPLE_CPP__) || defined(__APPLE_CC__) || defined(__MACOS_CLASSIC__) )
#include <TargetConditionals.h>
#if TARGET_OS_IPHONE == 1 || TARGET_IPHONE_SIMULATOR == 1
#define RUDE_IPHONE
#else
#define RUDE_MACOS
#define RUDE_IPAD 1
#endif
#endif

#ifndef RUDE_IPAD
#define RUDE_IPAD 0
#endif

// -------------------------------------------------------------------------

#ifdef RUDE_GAPI

#ifdef WIN32_PLATFORM_WFSP
#define RUDE_NOSTYLUS
#endif

#endif // RUDE_GAPI

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

#endif // RUDE_PALM

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

#endif // RUDE_GAPI







#ifdef RUDE_IPHONE

#define RUDE_OGLES
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TCHAR char
#define _stprintf sprintf
#define _tcscat strcat
#define _tcscpy strcpy
#define _tcslen strlen

#define _T(a) a

typedef unsigned long long u64;
typedef unsigned int WORD;

#endif // RUDE_IPHONE

#ifdef RUDE_MACOS

#define RUDE_OGL
#include <OpenGL/OpenGl.h>
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
typedef int GLfixed;
typedef unsigned int GLuint;
typedef float GLfloat;
typedef unsigned int WORD;


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TCHAR char
#define _stprintf sprintf
#define _tcscat strcat
#define _tcscpy strcpy
#define _tcslen strlen

#define _T(a) a

typedef unsigned long long u64;

#endif // RUDE_MACOS

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
	vsnprintf(text, 512, pszFormat, args);
	va_end(args);
	
	printf("ASSERT: %s (%d): %s\n", file, line, text);
	
#ifdef RUDE_WIN
	char outputstr[1024];
	_snprintf(outputstr, 1024, "ASSERT: %s (%d): %s\n", file, line, text);

	OutputDebugString(outputstr);
	DebugBreak();
#endif

	exit(1);
}

#define RUDE_ASSERT(x, ...) if((x) == 0) RudeAssert(__FILE__, __LINE__, __VA_ARGS__)
#else
#define RUDE_ASSERT(x, y) (0)
#endif
#endif



#endif
