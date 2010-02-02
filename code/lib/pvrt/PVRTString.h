/******************************************************************************

 @File         PVRTString.h

 @Title        

 @Copyright    Copyright (C) 2007 - 2008 by Imagination Technologies Limited.

 @Platform     ANSI compatible

 @Description  A string class that can be used as drop-in replacement for
               std::string on platforms/compilers that don't provide a full C++
               standard library.

******************************************************************************/
#ifndef _PVRTSTRING_H_
#define _PVRTSTRING_H_

#include <stdio.h>
#define _USING_PVRTSTRING_

namespace PVRT
{
	class string
	{
	public:
		typedef	size_t	size_type;
		typedef	char value_type;
		typedef	char& reference;
		typedef	const char& const_reference;

		static const size_type npos;

		string(const char* _Ptr, size_t _Count = npos);
		string(const string& _Right, size_t _Roff = 0, size_t _Count = npos);
		string(size_t _Count, const char _Ch);
		string(const char _Ch);
		string();
		//template <class InputIterator > string(InputIterator _First, InputIterator _Last, const allocator_type& _Al = Allocator());

		virtual ~string();

		string& append(const char* _Ptr);
		string& append(const char* _Ptr, size_t _Count);
		string& append(const string& _Str);
		string& append(const string& _Str, size_t _Off, size_t _Count);
		string& append(const char _Ch, size_t _Count);
		//template<class InputIterator> string& append(InputIterator _First, InputIterator _Last);

		string& assign(const char* _Ptr);
		string& assign(const char* _Ptr, size_t _Count);
		string& assign(const string& _Str);
		string& assign(const string& _Str, size_t _Off, size_t _Count=npos);
		string& assign(const char _Ch, size_t _Count=1);
		//template<class InputIterator> string& assign(InputIterator _First, InputIterator _Last);

		//const_reference at(size_t _Off) const;
		//reference at(size_t _Off);

		// const_iterator begin() const;
		// iterator begin();

		const char* c_str() const;

		size_t capacity() const;

		void clear();

		int compare(const string& _Str) const;
		int compare(size_t _Pos1, size_t _Num1, const string& _Str) const;
		int compare(size_t _Pos1, size_t _Num1, const string& _Str, size_t _Off, size_t _Count) const;
		int compare(const char* _Ptr) const;
		int compare(size_t _Pos1, size_t _Num1, const char* _Ptr) const;
		int compare(size_t _Pos1, size_t _Num1, const char* __Ptr, size_t _Num2 = npos) const;

		bool operator==(const string& _Str) const;
		bool operator==(const char* const _Ptr) const;

		size_t copy(char* _Ptr, size_t _Count, size_t _Off = 0) const;

		const char* data( ) const;

		bool empty() const;

		// const_iterator end() const;
		// iterator end();

		//iterator erase(iterator _First, iterator _Last);
		//iterator erase(iterator _It);
		string& erase(size_t _Pos = 0, size_t _Count = npos);

		//size_t find(char _Ch, size_t _Off = 0) const;
		//size_t find(const char* _Ptr, size_t _Off = 0) const;
		//size_t find(const char* _Ptr, size_t _Off = 0, size_t _Count) const;
		//size_t find(const string& _Str, size_t _Off = 0) const;

		size_t find_first_not_of(char _Ch, size_t _Off = 0) const;
		size_t find_first_not_of(const char* _Ptr, size_t _Off = 0) const;
		size_t find_first_not_of(const char* _Ptr, size_t _Off, size_t _Count) const;
		size_t find_first_not_of(const string& _Str, size_t _Off = 0) const;

		size_t find_first_of(char _Ch, size_t _Off = 0) const;
		size_t find_first_of(const char* _Ptr, size_t _Off = 0) const;
		size_t find_first_of(const char* _Ptr, size_t _Off, size_t _Count) const;
		size_t find_first_of(const string& _Str, size_t _Off = 0) const;

		size_t find_last_not_of(char _Ch, size_t _Off = 0) const;
		size_t find_last_not_of(const char* _Ptr, size_t _Off = 0) const;
		size_t find_last_not_of(const char* _Ptr, size_t _Off, size_t _Count) const;
		size_t find_last_not_of(const string& _Str, size_t _Off = 0) const;

		size_t find_last_of(char _Ch, size_t _Off = 0) const;
		size_t find_last_of(const char* _Ptr, size_t _Off = 0) const;
		size_t find_last_of(const char* _Ptr, size_t _Off, size_t _Count) const;
		size_t find_last_of(const string& _Str, size_t _Off = 0) const;

		//allocator_type get_allocator( ) const;

		string& insert(size_t _P0, const char* _Ptr);
		string& insert(size_t _P0, const char* _Ptr, size_t _Count);
		string& insert(size_t _P0, const string& _Str);
		string& insert(size_t _P0, const string& _Str, size_t _Off, size_t _Count);
		string& insert(size_t _P0, size_t _Count, char _Ch);
		//iterator insert(iterator _It, char _Ch = char());
		//template<class InputIterator> void insert(iterator _It, InputIterator _First, InputIterator _Last);
		//void insert(iterator _It, size_t _Count, char _Ch);

		size_t length() const;

		size_t max_size() const;

		void push_back(char _Ch);

		// const_reverse_iterator rbegin() const;
		// reverse_iterator rbegin();

		// const_reverse_iterator rend() const;
		// reverse_iterator rend();

		//string& replace(size_t _Pos1, size_t _Num1, const char* _Ptr);
		//string& replace(size_t _Pos1, size_t _Num1, const string& _Str);
		//string& replace(size_t _Pos1, size_t _Num1, const char* _Ptr, size_t _Num2);
		//string& replace(size_t _Pos1, size_t _Num1, const string& _Str, size_t _Pos2, size_t _Num2);
		//string& replace(size_t _Pos1, size_t _Num1, size_t _Count, char _Ch);

		//string& replace(iterator _First0, iterator _Last0, const char* _Ptr);
		//string& replace(iterator _First0, iterator _Last0, const string& _Str);
		//string& replace(iterator _First0, iterator _Last0, const char* _Ptr, size_t _Num2);
		//string& replace(iterator _First0, iterator _Last0, size_t _Num2, char _Ch);
		//template<class InputIterator> string& replace(iterator _First0, iterator _Last0, InputIterator _First, InputIterator _Last);

		void reserve(size_t _Count = 0);

		void resize(size_t _Count, char _Ch = char());

		//size_t rfind(char _Ch, size_t _Off = npos) const;
		//size_t rfind(const char* _Ptr, size_t _Off = npos) const;
		//size_t rfind(const char* _Ptr, size_t _Off = npos, size_t _Count) const;
		//size_t rfind(const string& _Str, size_t _Off = npos) const;

		size_t size() const;

		string substr(size_t _Off = 0, size_t _Count = npos) const;

		void swap(string& _Str);

		string& operator+=(char _Ch);
		string& operator+=(const char* _Ptr);
		string& operator+=(const string& _Right);

		string& operator=(char _Ch);
		string& operator=(const char* _Ptr);
		string& operator=(const string& _Right);

		const_reference operator[](size_t _Off) const;
		reference operator[](size_t _Off);

		friend string operator+ (const string& _Left, const string& _Right);
		friend string operator+ (const string& _Left, const char* _Right);
		friend string operator+ (const string& _Left, const char _Right);
		friend string operator+ (const char* _Left, const string& _Right);
		friend string operator+ (const char _Left, const string& _Right);

	protected:
		char* m_pString;
		size_t m_Size;
		size_t m_Capacity;
	};

	/*************************************************************************
	* MISCELLANEOUS UTILITY FUNCTIONS
	*************************************************************************/
	/*!***********************************************************************
	@Function			getExtension
	@Description		Extracts the file extension from a file path.
						Returns an empty string if no extension is found.
	************************************************************************/
	string getFileExtension(const string& strFilePath);

	/*!***********************************************************************
	@Function			getExtension
	@Description		Extracts the directory portion from a file path.
	************************************************************************/
	string GetContainingDirectoryPath(const string& strFilePath);
	/*!***********************************************************************
	@Function			getExtension
	@Description		Extracts the name and extension portion from a file path.
	************************************************************************/
	string GetFileName(const string& strFilePath);

	/*!***********************************************************************
	@Function			getExtension
	@Description		strips white space characters from the beginning of a string.
	************************************************************************/
	string StripWhiteSpaceFromStartOf(const string& strLine);
}

#endif // _PVRTSTRING_H_

/*****************************************************************************
End of file (PVRTString.h)
*****************************************************************************/
