/******************************************************************************

 @File         PVRTResourceFile.h

 @Title        

 @Copyright    Copyright (C) 2007 - 2008 by Imagination Technologies Limited.

 @Platform     ANSI compatible

 @Description  Simple resource file wrapper

******************************************************************************/
#ifndef _PVRTRESOURCEFILE_H_
#define _PVRTRESOURCEFILE_H_

#include <stdlib.h>
#include "PVRTString.h"

class CPVRTResourceFile
{
public:
	static void SetReadPath(const char* pszReadPath);
	static PVRT::string GetReadPath();

	CPVRTResourceFile(const char* pszFilename);
	virtual ~CPVRTResourceFile();

	bool IsOpen() const;
	bool IsMemoryFile() const;
	size_t Size() const;

	const void* DataPtr() const;
	// convenience getter. Also makes it clear that you get a null-terminated buffer.
	const char* StringPtr() const;

	void Close();

protected:
	bool m_bOpen;
	bool m_bMemoryFile;
	size_t m_Size;
	const char* m_pData;

	static PVRT::string s_ReadPath;
};

#endif // _PVRTRESOURCEFILE_H_

/*****************************************************************************
 End of file (PVRTResourceFile.h)
*****************************************************************************/
