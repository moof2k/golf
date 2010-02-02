/******************************************************************************

 @File         PVRTSingleton.h

 @Title        

 @Copyright    Copyright (C) 2003 - 2008 by Imagination Technologies Limited.

 @Platform     ANSI compatible

 @Description  Singleton template Pattern Usage: Inherit from singleton class
               like this: class Foo : public singleton<Foo> { ... };

******************************************************************************/
#ifndef __PVRTSINGLETON__
#define __PVRTSINGLETON__

namespace PVRT
{
	template<typename T> class singleton
	{
	public:
		static T& inst()
		{
			static T object;
			return object;
		}

		static T* ptr()
		{
			return &inst();
		}

	protected:
		singleton() {};
		virtual ~singleton() {};
	};
}

#endif // __PVRTSINGLETON__

/*****************************************************************************
End of file (PVRTSingleton.h)
*****************************************************************************/
