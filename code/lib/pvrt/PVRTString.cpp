/******************************************************************************

 @File         PVRTString.cpp

 @Title        

 @Copyright    Copyright (C) 2003 - 2008 by Imagination Technologies Limited.

 @Platform     ANSI compatible

 @Description  like this: class Foo : public singleton<Foo> { ... };

******************************************************************************/
#include "PVRTString.h"

#ifdef _USING_PVRTSTRING_

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "PVRTGlobal.h"

namespace PVRT
{
	const size_t string::npos = (size_t) -1;

	string::string(const char* _Ptr, size_t _Count) :
	m_pString(0), m_Capacity(0)
	{
		if (_Count == npos)
			assign(_Ptr);
		else
			assign(_Ptr, _Count);
	}

	string::string(const string& _Right, size_t _Roff, size_t _Count) :
	m_pString(0), m_Capacity(0)
	{
		assign(_Right, _Roff, _Count);
	}

	string::string(size_t _Count, char _Ch) :
	m_pString(0), m_Capacity(0)
	{
		assign(_Ch, _Count);
	}

	string::string(const char _Ch) :
	m_pString(0), m_Capacity(0)
	{
		assign( 1, _Ch);
	}

	string::string() :
	m_Size(0), m_Capacity(1)
	{
		m_pString = (char*)calloc(1, 1);
	}

	//	virtual string::~string()
	string::~string()
	{
		free(m_pString);
	}

	string& string::append(const char* _Ptr)
	{
		return append(_Ptr,strlen(_Ptr));
	}

	string& string::append(const char* _Ptr, size_t _Count)
	{
		char* pString = m_pString;
		size_t newCapacity = _Count + m_Capacity;

		// extend string if necessary
		if (m_Capacity < newCapacity)
		{
			pString = (char*)malloc(newCapacity);
			m_Capacity = newCapacity;
			memmove(pString, m_pString, m_Size);
			pString[m_Capacity-1]='\0';
		}

		// append chars from _Ptr
		memmove(pString + m_Size, _Ptr, _Count);
		m_Size += _Count;
		pString[m_Size] = 0;

		// remove old string if necessary
		if (pString != m_pString)
		{
			free(m_pString);
			m_pString = pString;
		}
		return *this;
	}

	string& string::append(const string& _Str)
	{
		return append(_Str.m_pString,_Str.m_Size);
	}

	// append _Count chars of _Str from _Off in _Str
	string& string::append(const string& _Str, size_t _Off, size_t _Count)
	{
		return append(_Str.m_pString+_Off,_Count);
	}

	string& string::append(char _Ch, size_t _Count)
	{
		char* pString = m_pString;
		size_t newCapacity = _Count + m_Size + 1;
		// extend string if necessary
		if (m_Capacity < newCapacity)
		{
			pString = (char*)malloc(newCapacity);
			m_Capacity = newCapacity;
			memmove(pString, m_pString, m_Size);
		}

		char* newChar = &pString[m_Size];
		// fill new space with _Ch
		for(size_t i=0;i<_Count;++i)
		{
			*newChar++ = _Ch;
		}

		// remove old string if necessary
		if (pString != m_pString)
		{
			free(m_pString);
			m_pString = pString;
		}
		return *this;
	}

	string& string::assign(const char* _Ptr)
	{
		return assign(_Ptr, strlen(_Ptr));
	}

	string& string::assign(const char* _Ptr, size_t _Count)
	{
		char* pString = m_pString;
		if (m_Capacity <= _Count)
		{
			pString = (char*)malloc(_Count + 1);
			m_Capacity = _Count+1;
		}
		m_Size = _Count;

		memmove(pString, _Ptr, m_Size);
		pString[m_Size] = 0;

		if (pString != m_pString)
		{
			free(m_pString);
			m_pString = pString;
		}
		return *this;
	}

	string& string::assign(const string& _Str)
	{
		return assign(_Str.m_pString, _Str.m_Size);
	}

	string& string::assign(const string& _Str, size_t _Off, size_t _Count)
	{
		if(_Count==npos)
		{
			_Count = _Str.m_Size;
		}
		return assign(&_Str.m_pString[_Off], _Count);
	}

	string& string::assign(char _Ch, size_t _Count)
	{
		if (m_Capacity <= _Count)
		{
			free(m_pString);
			m_pString = (char*)malloc(_Count + 1);
			m_Capacity = _Count+1;
		}
		m_Size = _Count;
		memset(m_pString, _Ch, _Count);
		m_pString[m_Size] = 0;

		return *this;
	}

	//const_reference at(size_t _Off) const;
	//reference at(size_t _Off);

	const char* string::c_str() const
	{
		return m_pString;
	}

	size_t string::capacity() const
	{
		return m_Capacity;
	}

	void string::clear()
	{
		free(m_pString);
		m_pString = (char*)calloc(1, 1);
		m_Size = 0;
		m_Capacity = 1;
	}

	int string::compare(const string& _Str) const
	{
		return strcmp(m_pString,_Str.m_pString);
	}

	int string::compare(size_t _Pos1, size_t _Num1, const string& _Str) const
	{
		assert(_Pos1<=m_Size);	// check comparison starts within lhs string

		int i32Ret;	// value to return if no difference in actual comparisons between chars
		size_t stLhsLength = m_Size-_Pos1;
		size_t stSearchLength = PVRT_MIN(stLhsLength,PVRT_MIN(_Str.m_Size,_Num1));	// number of comparisons to do
		if(PVRT_MIN(stLhsLength,_Num1)<PVRT_MIN(_Str.m_Size,_Num1))
		{
			i32Ret = -1;
		}
		else if(PVRT_MIN(stLhsLength,_Num1)>PVRT_MIN(_Str.m_Size,_Num1))
		{
			i32Ret = 1;
		}
		else
		{
			i32Ret = 0;
		}

		// do actual comparison
		const char* lhptr = &m_pString[_Pos1];
		const char* rhptr = _Str.m_pString;
		for(size_t i=0;i<stSearchLength;++i)
		{
			if(*lhptr<*rhptr)
				return -1;
			else if (*lhptr>*rhptr)
				return 1;
			lhptr++;rhptr++;
		}
		// no difference found in compared characters
		return i32Ret;
	}

	int string::compare(size_t _Pos1, size_t _Num1, const string& _Str, size_t _Off, size_t _Count) const
	{
		assert(_Pos1<=m_Size);	// check comparison starts within lhs string

		int i32Ret;	// value to return if no difference in actual comparisons between chars
		size_t stLhsLength = m_Size-_Pos1;
		size_t stSearchLength = PVRT_MIN(stLhsLength,PVRT_MIN(_Str.m_Size,PVRT_MIN(_Num1,_Count)));	// number of comparisons to do
		if(PVRT_MIN(stLhsLength,_Num1)<PVRT_MIN(_Str.m_Size,_Count))
		{
			i32Ret = -1;
		}
		else if(PVRT_MIN(stLhsLength,_Num1)>PVRT_MIN(_Str.m_Size,_Count))
		{
			i32Ret = 1;
		}
		else
		{
			i32Ret = 0;
		}


		// do actual comparison
		char* lhptr = &m_pString[_Pos1];
		char* rhptr = _Str.m_pString;
		for(size_t i=0;i<stSearchLength;++i)
		{
			if(*lhptr<*rhptr)
				return -1;
			else if (*lhptr>*rhptr)
				return 1;
			lhptr++;rhptr++;
		}
		// no difference found in compared characters
		return i32Ret;
	}

	int string::compare(const char* _Ptr) const
	{
		return strcmp(m_pString,_Ptr);
	}

	int string::compare(size_t _Pos1, size_t _Num1, const char* _Ptr) const
	{
		assert(_Pos1<=m_Size);	// check comparison starts within lhs string

		int i32Ret;	// value to return if no difference in actual comparisons between chars
		size_t stLhsLength = m_Size-_Pos1;
		size_t stRhsLength = strlen(_Ptr);
		size_t stSearchLength = PVRT_MIN(stLhsLength,PVRT_MIN(stRhsLength,_Num1));	// number of comparisons to do
		if(PVRT_MIN(stLhsLength,_Num1)<PVRT_MIN(stRhsLength,_Num1))
		{
			i32Ret = -1;
		}
		else if(PVRT_MIN(stLhsLength,_Num1)>PVRT_MIN(stRhsLength,_Num1))
		{
			i32Ret = 1;
		}
		else
		{
			i32Ret = 0;
		}

		// do actual comparison
		const char* lhptr = &m_pString[_Pos1];
		const char* rhptr = _Ptr;
		for(size_t i=0;i<stSearchLength;++i)
		{
			if(*lhptr<*rhptr)
				return -1;
			else if (*lhptr>*rhptr)
				return 1;
			lhptr++;rhptr++;
		}
		// no difference found in compared characters
		return i32Ret;
	}

	int string::compare(size_t _Pos1, size_t _Num1, const char* _Ptr, size_t _Count) const
	{
		assert(_Pos1<=m_Size);	// check comparison starts within lhs string

		int i32Ret;	// value to return if no difference in actual comparisons between chars
		size_t stLhsLength = m_Size-_Pos1;
		size_t stRhsLength = strlen(_Ptr);
		size_t stSearchLength = PVRT_MIN(stLhsLength,PVRT_MIN(stRhsLength,PVRT_MIN(_Num1,_Count)));	// number of comparisons to do
		if(PVRT_MIN(stLhsLength,_Num1)<PVRT_MIN(stRhsLength,_Count))
		{
			i32Ret = -1;
		}
		else if(PVRT_MIN(stLhsLength,_Num1)>PVRT_MIN(stRhsLength,_Count))
		{
			i32Ret = 1;
		}
		else
		{
			i32Ret = 0;
		}


		// do actual comparison
		char* lhptr = &m_pString[_Pos1];
		const char* rhptr = _Ptr;
		for(size_t i=0;i<stSearchLength;++i)
		{
			if(*lhptr<*rhptr)
				return -1;
			else if (*lhptr>*rhptr)
				return 1;
			lhptr++;rhptr++;
		}
		// no difference found in compared characters
		return i32Ret;
	}

	//_Str
	//
	//    The string that is to be compared to the operand string.
	//
	//_Pos1
	//
	//    The index of the operand string at which the comparison begins.
	//
	//_Num1
	//
	//    The maximum number of characters from the operand string to be compared.
	//
	//_Num2
	//
	//    The maximum number of characters from the parameter string to be compared.
	//
	//_Off
	//
	//    The index of the parameter string at which the comparison begins.
	//
	//_Count
	//
	//    The maximum number of characters from the parameter string to be compared.
	//
	//_Ptr
	//
	//    The C-string to be compared to the operand string.


	bool string::operator==(const string& _Str) const
	{
		return strcmp(m_pString, _Str.m_pString)==0;
	}
	bool string::operator==(const char* const _Ptr) const
	{
		return strcmp(m_pString, _Ptr)==0;
	}

	size_t string::copy(char* _Ptr, size_t _Count, size_t _Off) const
	{
		if(memcpy(_Ptr, &m_pString[_Off], PVRT_MIN(_Count, m_Size - _Off)))
			return _Count;

		return 0;
	}

	const char* string::data() const
	{
		return m_pString;
	}

	bool string::empty() const
	{
		return (m_Size == 0);
	}

	string& string::erase(size_t _Pos, size_t _Count)
	{
		if (_Count == npos || _Pos + _Count >= m_Size)
		{
			resize(_Pos, 0);
		}
		else
		{
			memmove(&m_pString[_Pos], &m_pString[_Pos + _Count], m_Size + 1 - (_Pos + _Count));
		}
		return *this;
	}

	//size_t find(char _Ch, size_t _Off = 0) const;
	//size_t find(const char* _Ptr, size_t _Off = 0) const;
	//size_t find(const char* _Ptr, size_t _Off = 0, size_t _Count) const;
	//size_t find(const string& _Str, size_t _Off = 0) const;

	size_t string::find_first_not_of(char _Ch, size_t _Off) const
	{
		for(size_t i=_Off;i<m_Size;++i)
		{
			if(m_pString[i]!=_Ch)
				return i;
		}
		return npos;
	}

	size_t string::find_first_not_of(const char* _Ptr, size_t _Off) const
	{
		for(size_t i=_Off;i<m_Size;++i)
		{
			bool bFound = false;
			// compare against each char from _Ptr
			for(size_t j=0;_Ptr[j]!=0;++j)
			{
				bFound = bFound || (m_pString[i]!=_Ptr[j]);
			}
			if(!bFound)
			{	// return if no match
				return i;
			}
		}
		return npos;
	}

	size_t string::find_first_not_of(const char* _Ptr, size_t _Off, size_t _Count) const
	{
		for(size_t i=_Off;i<m_Size;++i)
		{
			bool bFound = false;
			// compare against each char from _Ptr
			for(size_t j=0;j<_Count;++j)
			{
				bFound = bFound || (m_pString[i]!=_Ptr[j]);
			}
			if(!bFound)
			{	// return if no match
				return i;
			}
		}
		return npos;
	}

	size_t string::find_first_not_of(const string& _Str, size_t _Off) const
	{
		for(size_t i=_Off;i<m_Size;++i)
		{
			bool bFound = false;
			// compare against each char from _Str
			for(size_t j=0;j<_Str.m_Size;++j)
			{
				bFound = bFound || (m_pString[i]!=_Str[j]);
			}
			if(!bFound)
			{	// return if no match
				return i;
			}
		}
		return npos;
	}

	size_t string::find_first_of(char _Ch, size_t _Off) const
	{
		for(size_t i=_Off;i<m_Size;++i)
		{
			if(m_pString[i]==_Ch)
				return i;
		}
		return npos;
	}

	size_t string::find_first_of(const char* _Ptr, size_t _Off) const
	{
		for(size_t i=_Off;i<m_Size;++i)
		{
			// compare against each char from _Ptr
			for(size_t j=0;_Ptr[j]!=0;++j)
			{
				if(m_pString[i]==_Ptr[j])
					return i;
			}
		}
		return npos;
	}

	size_t string::find_first_of(const char* _Ptr, size_t _Off, size_t _Count) const
	{
		for(size_t i=_Off;i<m_Size;++i)
		{
			// compare against each char from _Ptr
			for(size_t j=0;j<_Count;++j)
			{
				if(m_pString[i]==_Ptr[j])
					return i;
			}
		}
		return npos;
	}

	size_t string::find_first_of(const string& _Str, size_t _Off) const
	{
		for(size_t i=_Off;i<m_Size;++i)
		{
			// compare against each char from _Ptr
			for(size_t j=0;j<_Str.m_Size;++j)
			{
				if(m_pString[i]==_Str[j])
					return i;
			}
		}
		return npos;
	}

	size_t string::find_last_not_of(char _Ch, size_t _Off) const
	{
		for(size_t i=m_Size-_Off-1;i<m_Size;--i)
		{
			if(m_pString[i]!=_Ch)
			{
				return i;
			}
		}
		return npos;
	}

	size_t string::find_last_not_of(const char* _Ptr, size_t _Off) const
	{
		for(size_t i=m_Size-_Off-1;i<m_Size;--i)
		{
			bool bFound = false;
			// compare against each char from _Ptr
			for(size_t j=0;_Ptr[j]!=0;++j)
			{
				bFound = bFound || (m_pString[i]!=_Ptr[j]);
			}
			if(!bFound)
			{	// return if no match
				return i;
			}
		}
		return npos;
	}

	size_t string::find_last_not_of(const char* _Ptr, size_t _Off, size_t _Count) const
	{
		for(size_t i=m_Size-_Off-1;i<m_Size;--i)
		{
			bool bFound = false;
			// compare against each char from _Ptr
			for(size_t j=0;j<_Count;++j)
			{
				bFound = bFound || (m_pString[i]!=_Ptr[j]);
			}
			if(!bFound)
			{	// return if no match
				return i;
			}
		}
		return npos;
	}

	size_t string::find_last_not_of(const string& _Str, size_t _Off) const
	{
		for(size_t i=m_Size-_Off-1;i<m_Size;--i)
		{
			bool bFound = false;
			// compare against each char from _Ptr
			for(size_t j=0;j<_Str.m_Size;++j)
			{
				bFound = bFound || (m_pString[i]!=_Str[j]);
			}
			if(!bFound)
			{	// return if no match
				return i;
			}
		}
		return npos;
	}

	size_t string::find_last_of(char _Ch, size_t _Off) const
	{
		for(size_t i=m_Size-_Off-1;i<m_Size;--i)
		{
			if(m_pString[i]==_Ch)
			{
				return i;
			}
		}
		return npos;
	}

	size_t string::find_last_of(const char* _Ptr, size_t _Off) const
	{
		for(size_t i=m_Size-_Off-1;i<m_Size;--i)
		{
			// compare against each char from _Ptr
			for(size_t j=0;_Ptr[j]!=0;++j)
			{
				if(m_pString[i]==_Ptr[j])
					return i;
			}
		}
		return npos;
	}

	size_t string::find_last_of(const char* _Ptr, size_t _Off, size_t _Count) const
	{
		for(size_t i=m_Size-_Off-1;i<m_Size;--i)
		{
			// compare against each char from _Ptr
			for(size_t j=0;j<_Count;++j)
			{
				if(m_pString[i]!=_Ptr[j])
					return i;
			}
		}
		return npos;
	}

	size_t string::find_last_of(const string& _Str, size_t _Off) const
	{
		for(size_t i=m_Size-_Off-1;i<m_Size;--i)
		{
			// compare against each char from _Str
			for(size_t j=0;j<_Str.m_Size;++j)
			{
				if(m_pString[i]!=_Str[j])
					return i;
			}
		}
		return npos;
	}

	//string& string::insert(size_t _P0, const char* _Ptr)
	//{
	//	return replace(_P0, 0, _Ptr);
	//}

	//string& string::insert(size_t _P0, const char* _Ptr, size_t _Count)
	//{
	//	return replace(_P0, 0, _Ptr, _Count);
	//}

	//string& string::insert(size_t _P0, const string& _Str)
	//{
	//	return replace(_P0, 0, _Str);
	//}

	//string& string::insert(size_t _P0, const string& _Str, size_t _Off, size_t _Count)
	//{
	//	return replace(_P0, 0, _Str, _Off, _Count);
	//}

	//string& string::insert(size_t _P0, size_t _Count, char _Ch)
	//{
	//	return replace(_P0, 0, _Count, _Ch);
	//}

	size_t string::length() const
	{
		return m_Size;
	}

	size_t string::max_size() const
	{
		return 0x7FFFFFFF;
	}

	void string::push_back(char _Ch)
	{
		append(_Ch, 1);
	}

	//string& replace(size_t _Pos1, size_t _Num1, const char* _Ptr)
	//string& replace(size_t _Pos1, size_t _Num1, const string& _Str)
	//string& replace(size_t _Pos1, size_t _Num1, const char* _Ptr, size_t _Num2)
	//string& replace(size_t _Pos1, size_t _Num1, const string& _Str, size_t _Pos2, size_t _Num2)
	//string& replace(size_t _Pos1, size_t _Num1, size_t _Count, char _Ch)

	void string::reserve(size_t _Count)
	{
		if (_Count >= m_Capacity)
		{
			m_pString = (char*)realloc(m_pString, _Count + 1);
			m_Capacity = _Count + 1;
		}
	}

	void string::resize(size_t _Count, char _Ch)
	{
		if (_Count <= m_Size)
		{
			m_Size = _Count;
			m_pString[m_Size] = 0;
		}
		else
		{
			append(_Ch, _Count - m_Size);
		}
	}

	//size_t rfind(char _Ch, size_t _Off = npos) const;
	//size_t rfind(const char* _Ptr, size_t _Off = npos) const;
	//size_t rfind(const char* _Ptr, size_t _Off = npos, size_t _Count) const;
	//size_t rfind(const string& _Str, size_t _Off = npos) const;

	size_t string::size() const
	{
		return m_Size;
	}

	string string::substr(size_t _Off, size_t _Count) const
	{
		return string(*this, _Off, _Count);
	}

	void string::swap(string& _Str)
	{
		size_t Size = _Str.m_Size;
		size_t Capacity = _Str.m_Capacity;
		char* pString = _Str.m_pString;
		_Str.m_Size = m_Size;
		_Str.m_Capacity = m_Capacity;
		_Str.m_pString = m_pString;
		m_Size = Size;
		m_Capacity = Capacity;
		m_pString = pString;
	}

	string& string::operator+=(char _Ch)
	{
		return append(_Ch, 1);
	}

	string& string::operator+=(const char* _Ptr)
	{
		return append(_Ptr);
	}

	string& string::operator+=(const string& _Right)
	{
		return append(_Right);
	}

	string& string::operator=(char _Ch)
	{
		return assign(_Ch, 1);
	}

	string& string::operator=(const char* _Ptr)
	{
		return assign(_Ptr);
	}

	string& string::operator=(const string& _Right)
	{
		return assign(_Right);
	}

	string::const_reference string::operator[](size_t _Off) const
	{
		return m_pString[_Off];
	}

	string::reference string::operator[](size_t _Off)
	{
		return m_pString[_Off];
	}


	string operator+ (const string& _Left, const string& _Right)
	{
		return string(_Left).append(_Right);
	}

	string operator+ (const string& _Left, const char* _Right)
	{
		return string(_Left).append(_Right);
	}

	string operator+ (const string& _Left, const char _Right)
	{
		return string(_Left).append(_Right);
	}

	string operator+ (const char* _Left, const string& _Right)
	{
		return string(_Left).append(_Right);
	}

	string operator+ (const char _Left, const string& _Right)
	{
		return string(_Left).append(_Right);
	}

/*************************************************************************
* MISCELLANEOUS UTILITY FUNCTIONS
*************************************************************************/
/*!***********************************************************************
@Function			getExtension
@Description		Extracts the file extension from a file path.
					Returns an empty string if no extension is found.
************************************************************************/
string getFileExtension(const string& strFilePath)
{
	string::size_type idx = strFilePath.find_last_of ( '.' );

    if (idx == string::npos)
    	return string("");
    else
    	return strFilePath.substr(idx);
}

/*!***********************************************************************
@Function			getExtension
@Description		Extracts the directory portion from a file path.
************************************************************************/
string GetContainingDirectoryPath(const string& strFilePath)
{
	size_t i32sep = strFilePath.find_last_of('/');
	if(i32sep == strFilePath.npos)
	{
		i32sep = strFilePath.find_last_of('\\');
		if(i32sep == strFilePath.npos)
		{	// can't find an actual \ or / so leave it be
			return strFilePath;
		}
	}
	return strFilePath.substr(0,i32sep);
}
/*!***********************************************************************
@Function			getExtension
@Description		Extracts the name and extension portion from a file path.
************************************************************************/
string GetFileName(const string& strFilePath)
{
	size_t i32sep = strFilePath.find_last_of('/');
	if(i32sep == strFilePath.npos)
	{
		i32sep = strFilePath.find_last_of('\\');
		if(i32sep == strFilePath.npos)
		{	// can't find an actual \ or / so leave it be
			return strFilePath;
		}
	}
	return strFilePath.substr(i32sep+1,strFilePath.length());
}

/*!***********************************************************************
@Function			getExtension
@Description		strips white space characters from the beginning of a string.
************************************************************************/
string StripWhiteSpaceFromStartOf(const string& strLine)
{
	size_t start = strLine.find_first_not_of(" \t	");
	if(start!=strLine.npos)
		return strLine.substr(start,strLine.length()-start+1);
	return strLine;
}
}

///*!***************************************************************************

#endif // _USING_PVRTSTRING_

