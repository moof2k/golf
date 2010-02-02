/******************************************************************************

 @File         PVRTTextureAPI.h

 @Title        

 @Copyright    Copyright (C) 2004 - 2008 by Imagination Technologies Limited.

 @Platform     ANSI compatible

 @Description  GL texture loading.

******************************************************************************/
#ifndef _PVRTTEXTUREAPI_H_
#define _PVRTTEXTUREAPI_H_

/****************************************************************************
** Functions
****************************************************************************/

/*!***************************************************************************
 @Function		PVRTLoadDecompressedTextureFromPointer
 @Modified		texName			the OpenGL ES texture name as returned by glBindTexture
 @Modified		psTextureHeader	Pointer to a PVR_Texture_Header struct. Modified to
								contain the header data of the returned texture Ignored if NULL.
 @Input			pointer			Pointer to the header-prefixed texture
 @Return		true on success
 @Description	Allows textures to be stored in header files and loaded in. Loads the whole texture
				Release texture by calling PVRTReleaseTexture.  Decompresses to RGBA8888 internally.
*****************************************************************************/
unsigned int PVRTLoadDecompressedTextureFromPointer(const void* pointer, GLuint *texName, const void *psTextureHeader=NULL);

/*!***************************************************************************
 @Function		PVRTLoadDecompressedPartialTextureFromPointer
 @Input			pointer			Pointer to the header-prefixed texture from
 @Input			nLoadFromLevel	Which mipmap level to start loading from (0=all)
 @Modified		texName			the OpenGL ES texture name as returned by glBindTexture
 @Modified		psTextureHeader	Pointer to a PVR_Texture_Header struct. Modified to
								contain the header data of the returned texture Ignored if NULL.	
 @Return		true on success
 @Description	Allows textures to be stored in header files and loaded in. Loads the whole texture
				Release texture by calling PVRTReleaseTexture.  Decompresses to RGBA8888 internally.
*****************************************************************************/
unsigned int PVRTLoadDecompressedPartialTextureFromPointer(const void *pointer,
														   unsigned int nLoadFromLevel,
														   GLuint *texName,
														   const void *psTextureHeader=NULL);

/*!***************************************************************************
 @Function		PVRTLoadPartialTextureFromPointer
 @Input			pointer			Pointer to header-texture's structure
 @Input			texPtr			If null, texture follows header, else texture is here.
 @Input			nLoadFromLevel	Which mipmap level to start loading from (0=all)
 @Modified		texName			the OpenGL ES texture name as returned by glBindTexture
 @Modified		psTextureHeader	Pointer to a PVR_Texture_Header struct. Modified to
								contain the header data of the returned texture Ignored if NULL.	
 @Return		true on success
 @Description	Allows textures to be stored in C header files and loaded in.  Can load parts of a
				mipmaped texture (ie skipping the highest detailed levels).  Release texture by calling
				PVRTReleaseTexture.
*****************************************************************************/
unsigned int PVRTLoadPartialTextureFromPointer(const void * const pointer,
											   const void * const texPtr,
											   const unsigned int nLoadFromLevel,
											   GLuint * const texName,
											   const void *psTextureHeader=NULL);

/*!***************************************************************************
 @Function		OGLESShellLoadTextureFromPointer
 @Input			pointer			Pointer to header-texture's structure
 @Modified		texName			the OpenGL ES texture name as returned by glBindTexture
 @Modified		psTextureHeader	Pointer to a PVR_Texture_Header struct. Modified to
								contain the header data of the returned texture Ignored if NULL.	
 @Return		true on success
 @Description	Allows textures to be stored in C header files and loaded in.  Loads the whole texture.
				Release texture by calling PVRTReleaseTexture.
*****************************************************************************/
unsigned int PVRTLoadTextureFromPointer(const void* pointer, GLuint *const texName, const void *psTextureHeader=NULL);

/*!***************************************************************************
 @Function		PVRTLoadPartialTextureFromPVR
 @Input			filename		Filename of the .PVR file to load the texture from
 @Input			texPtr			If null, texture follows header, else texture is here.
 @Input			nLoadFromLevel	Which mipmap level to start loading from (0=all)
 @Modified		texName			the OpenGL ES texture name as returned by glBindTexture
 @Modified		psTextureHeader	Pointer to a PVR_Texture_Header struct. Modified to
								contain the header data of the returned texture Ignored if NULL.	
 @Return		true on success
 @Description	Allows textures to be stored in binary PVR files and loaded in. Can load parts of a
				mipmaped texture (ie skipping the highest detailed levels). Release texture by
				calling PVRTReleaseTexture.
*****************************************************************************/
unsigned int PVRTLoadPartialTextureFromPVR(const char * const filename,
										   const char * const texPtr,
										   const unsigned int nLoadFromLevel,
										   GLuint * const texName,
										   const void *psTextureHeader=NULL);

/*!***************************************************************************
 @Function		PVRTLoadTextureFromPVR
 @Input			filename		Filename of the .PVR file to load the texture from
 @Modified		texName			the OpenGL ES texture name as returned by glBindTexture
 @Modified		psTextureHeader	Pointer to a PVR_Texture_Header struct. Modified to
								contain the header data of the returned texture Ignored if NULL.	
 @Return		true on success
 @Description	Allows textures to be stored in binary PVR files and loaded in. Loads the whole texture
				Release texture by calling PVRTReleaseTexture.
*****************************************************************************/
unsigned int PVRTLoadTextureFromPVR(const char * const filename, GLuint * const texName, const void *psTextureHeader=NULL);

/*!***************************************************************************
 @Function		PVRTLoadDecompressedPartialTextureFromPVR
 @Input			filename		Filename of the .PVR file to load the texture from
 @Input			nLoadFromLevel	Which mipmap level to start loading from (0=all)
 @Modified		texName			the OpenGL ES texture name as returned by glBindTexture
 @Modified		psTextureHeader	Pointer to a PVR_Texture_Header struct. Modified to
								contain the header data of the returned texture Ignored if NULL.	
 @Return		true on success
 @Description	Allows textures to be stored in binary PVR files and loaded in. Can load parts of a
				mipmaped texture (ie skipping the highest detailed levels). Release texture by
				calling PVRTReleaseTexture.  This variant decompresses to RGBA8888.
*****************************************************************************/
unsigned int PVRTLoadDecompressedPartialTextureFromPVR(const char* const filename,
													   unsigned int nLoadFromLevel,
													   GLuint *texName,
													   const void *psTextureHeader=NULL);

/*!***************************************************************************
 @Function		PVRTLoadDecompressedTextureFromPVR
 @Input			filename		Filename of the .PVR file to load the texture from
 @Modified		texName			the OpenGL ES texture name as returned by glBindTexture
 @Modified		psTextureHeader	Pointer to a PVR_Texture_Header struct. Modified to
								contain the header data of the returned texture Ignored if NULL.	
 @Return		true on success
 @Description	Allows textures to be stored in binary PVR files and loaded in. Loads the whole texture
				Release texture by calling PVRTReleaseTexture.  This variant decompresses to RGBA8888.
*****************************************************************************/
unsigned int PVRTLoadDecompressedTextureFromPVR(const char* const filename,
												GLuint *const texName,
												const void *psTextureHeader=NULL);

/*!***************************************************************************
 @Function		PVRTReleaseTexture
 @Input			texName			the name returned by PVRTLoadTextureFromPointer or
								PVRTLoadTextureFromPVR
 @Description	Releases the resources used by a texture.
*****************************************************************************/
void PVRTReleaseTexture(GLuint texName);

/*!***************************************************************************
 @Function			PVRTTextureFormatBPP
 @Input				nFormat
 @Input				nType
 @Description		Returns the bits per pixel (BPP) of the format.
*****************************************************************************/
unsigned int PVRTTextureFormatBPP(const GLuint nFormat, const GLuint nType);


#endif /* _PVRTTEXTUREAPI_H_ */

/*****************************************************************************
 End of file (PVRTTextureAPI.h)
*****************************************************************************/
