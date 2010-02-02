/******************************************************************************

 @File         PVRTBackground.h

 @Title        

 @Copyright    Copyright (C) 2007 - 2008 by Imagination Technologies Limited.

 @Platform     ANSI compatible

 @Description  Function to draw a background texture.

******************************************************************************/
#ifndef __PVRTBACKGROUND_H__
#define __PVRTBACKGROUND_H__

#include "../PVRTGlobal.h"
#include "PVRTContext.h"
#include "../PVRTString.h"

/****************************************************************************
** Structures
****************************************************************************/
struct SPVRTBackgroundAPI;

/*!***************************************************************************
 Class: PVRT::Background
*****************************************************************************/
namespace PVRT
{
	class Background
	{
		public:
			/*****************************************************************************
			 @Function			Background
		 	 @Description		Init some values.
			*****************************************************************************/
			Background(void);
			/*****************************************************************************
			 @Function			~Background
			 @Description		Calls Destroy()
			*****************************************************************************/
			~Background(void);
			/*!***************************************************************************
			 @Function		Destroy
			 @Description	Destroys the background. It's called by the destructor.
			*****************************************************************************/
			void Destroy();
			/*!***************************************************************************
			 @Function		Init
			 @Input			pContext	A pointer to a PVRTContext
			 @Input			bRotate		true to rotate texture 90 degrees.
			 @Input			pszError	An option string for returning errors
			 @Description	Initialises the background
			*****************************************************************************/
			bool Init(const SPVRTContext * const pContext, const bool bRotate, PVRT::string *pszError = 0);
			/*!***************************************************************************
			 @Function		Draw
			 @Input			ui32Texture	Texture to use
			 @Description	Draws a texture on a quad covering the whole screen.
			*****************************************************************************/
			bool Draw(const GLuint ui32Texture);

		protected:
			bool m_bInit;
			SPVRTBackgroundAPI *m_pAPI;
	};
}

#endif /* __PVRTBACKGROUND_H__ */

/*****************************************************************************
 End of file (PVRTBackground.h)
*****************************************************************************/
