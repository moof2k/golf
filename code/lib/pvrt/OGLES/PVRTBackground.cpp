/******************************************************************************

 @File         PVRTBackground.cpp

 @Title        

 @Copyright    Copyright (C) 1999 - 2008 by Imagination Technologies Limited.

 @Platform     ANSI compatible

 @Description  Function to draw a background texture.

******************************************************************************/
#include <string.h>
#include "PVRTFixedPoint.h"
#include "PVRTBackground.h"

/****************************************************************************
** Structures
****************************************************************************/

// The struct to include various API variables
struct SPVRTBackgroundAPI
{
	VERTTYPE m_aVertexData[20];
	GLsizei m_i32Stride;
	VERTTYPE *m_pVertexPtr;
	VERTTYPE *m_pTexturePtr;
};

/****************************************************************************
** Class: PVRT::Background
****************************************************************************/

/*****************************************************************************
 @Function			Background
 @Description		Init some values.
*****************************************************************************/
PVRT::Background::Background(void)
{
	m_bInit = false;
	m_pAPI  = 0;
}

/*****************************************************************************
 @Function			~Background
 @Description		Calls Destroy()
*****************************************************************************/
PVRT::Background::~Background(void)
{
	Destroy();
}

/*!***************************************************************************
 @Function		Destroy
 @Description	Destroys the background. It's called by the destructor.
*****************************************************************************/
void PVRT::Background::Destroy()
{
	m_bInit = false;

	delete m_pAPI;
	m_pAPI = 0;
}

/*!***************************************************************************
 @Function		Init
 @Input			pContext	A pointer to a PVRTContext
 @Input			bRotate		true to rotate texture 90 degrees.
 @Input			pszError	An option string for returning errors
 @Description	Initialises the background
*****************************************************************************/
bool PVRT::Background::Init(const SPVRTContext * const pContext, bool bRotate, PVRT::string *pszError)
{
	Destroy();

	m_pAPI = new SPVRTBackgroundAPI;

	if(!m_pAPI)
	{
		if(pszError)
			*pszError = "Error: Insufficient memory to allocate SPVRTBackgroundAPI.";

		return false;
	}

	// The vertex data for non-rotated
	VERTTYPE afVertexData[20] = {f2vt(-1.0f), f2vt(-1.0f), f2vt(1.0f),  // Position
								 f2vt( 0.0f), f2vt( 0.0f),				// Texture coordinates
								 f2vt( 1.0f), f2vt(-1.0f), f2vt(1.0f),
								 f2vt( 1.0f), f2vt( 0.0f),
								 f2vt(-1.0f), f2vt( 1.0f), f2vt(1.0f),
								 f2vt( 0.0f), f2vt( 1.0f),
								 f2vt( 1.0f), f2vt( 1.0f), f2vt(1.0f),
								 f2vt( 1.0f), f2vt( 1.0f)};

	// The vertex data for rotated
	VERTTYPE afVertexDataRotated[20] = {f2vt(-1.0f), f2vt( 1.0f), f2vt(1.0f),
										f2vt( 1.1f), f2vt( 1.1f),
										f2vt(-1.0f), f2vt(-1.0f), f2vt(1.0f),
										f2vt( 0.0f), f2vt( 1.0f),
										f2vt( 1.0f), f2vt( 1.0f), f2vt(1.0f),
										f2vt( 1.0f), f2vt( 0.0f),
										f2vt( 1.0f), f2vt(-1.0f), f2vt(1.0f),
										f2vt( 0.0f), f2vt( 0.0f)};

	// Make a copy of the vertex data we want in the struct
	if(!bRotate)
		memcpy(&m_pAPI->m_aVertexData[0], &afVertexData[0], sizeof(VERTTYPE) * 20);
	else
		memcpy(&m_pAPI->m_aVertexData[0], &afVertexDataRotated[0], sizeof(VERTTYPE) * 20);

	// Setup the vertex and texture data pointers for conveniece
	m_pAPI->m_pVertexPtr  = &m_pAPI->m_aVertexData[0];
	m_pAPI->m_pTexturePtr = (VERTTYPE*) ((sizeof(VERTTYPE) * 3) + (size_t) &m_pAPI->m_aVertexData[0]);

	// Setup the stride variable
	m_pAPI->m_i32Stride   = sizeof(VERTTYPE) * 5;

	// All initialised
	m_bInit = true;
	return true;
}

/*!***************************************************************************
 @Function		Draw
 @Input			ui32Texture	Texture to use
 @Description	Draws a texture on a quad covering the whole screen.
*****************************************************************************/
bool PVRT::Background::Draw(const GLuint ui32Texture)
{
	if(!m_bInit)
		return false;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, ui32Texture);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	// Store matrices and set them to Identity
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glDisableClientState(GL_COLOR_ARRAY);

	// Set vertex data
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3  ,VERTTYPEENUM,m_pAPI->m_i32Stride, m_pAPI->m_pVertexPtr);

	// Set texture coordinates
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2,VERTTYPEENUM,m_pAPI->m_i32Stride, m_pAPI->m_pTexturePtr);

	// Render geometry
	glDrawArrays(GL_TRIANGLE_STRIP,0,4);

	// Disable client states
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	// Recover matrices
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	return true;
}

/*****************************************************************************
 End of file (PVRTBackground.cpp)
*****************************************************************************/
