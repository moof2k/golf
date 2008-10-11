/******************************************************************************

 @File         PVRTTexture.h

 @Title        

 @Copyright    Copyright (C) 2004 - 2008 by Imagination Technologies Limited.

 @Platform     ANSI compatible

 @Description  Texture loading.

******************************************************************************/
#ifndef _PVRTTEXTURE_H_
#define _PVRTTEXTURE_H_

#include "PVRTGlobal.h"
/*!***************************************************************************
 Macros
*****************************************************************************/

/*!***************************************************************************
 Describes the header of a PVR header-texture
*****************************************************************************/
typedef struct PVR_Header_Texture_TAG
{
	unsigned int dwHeaderSize;			/*!< size of the structure */
	unsigned int dwHeight;				/*!< height of surface to be created */
	unsigned int dwWidth;				/*!< width of input surface */
	unsigned int dwMipMapCount;			/*!< number of mip-map levels requested */
	unsigned int dwpfFlags;				/*!< pixel format flags */
	unsigned int dwTextureDataSize;		/*!< Total size in bytes */
	unsigned int dwBitCount;			/*!< number of bits per pixel  */
	unsigned int dwRBitMask;			/*!< mask for red bit */
	unsigned int dwGBitMask;			/*!< mask for green bits */
	unsigned int dwBBitMask;			/*!< mask for blue bits */
	unsigned int dwAlphaBitMask;		/*!< mask for alpha channel */
	unsigned int dwPVR;					/*!< magic number identifying pvr file */
	unsigned int dwNumSurfs;			/*!< the number of surfaces present in the pvr */
} PVR_Texture_Header;

/*****************************************************************************
 * ENUMS
 *****************************************************************************/

typedef enum PixelType_TAG
{
	MGLPT_ARGB_4444 = 0x00,
	MGLPT_ARGB_1555,
	MGLPT_RGB_565,
	MGLPT_RGB_555,
	MGLPT_RGB_888,
	MGLPT_ARGB_8888,
	MGLPT_ARGB_8332,
	MGLPT_I_8,
	MGLPT_AI_88,
	MGLPT_1_BPP,
	MGLPT_VY1UY0,
	MGLPT_Y1VY0U,
	MGLPT_PVRTC2,
	MGLPT_PVRTC4,
	MGLPT_PVRTC2_2,
	MGLPT_PVRTC2_4,

	OGL_RGBA_4444= 0x10,
	OGL_RGBA_5551,
	OGL_RGBA_8888,
	OGL_RGB_565,
	OGL_RGB_555,
	OGL_RGB_888,
	OGL_I_8,
	OGL_AI_88,
	OGL_PVRTC2,
	OGL_PVRTC4,

	// OGL_BGRA_8888 extension
	OGL_BGRA_8888,

	D3D_DXT1 = 0x20,
	D3D_DXT2,
	D3D_DXT3,
	D3D_DXT4,
	D3D_DXT5,

	D3D_RGB_332,
	D3D_AI_44,
	D3D_LVU_655,
	D3D_XLVU_8888,
	D3D_QWVU_8888,

	//10 bits per channel
	D3D_ABGR_2101010,
	D3D_ARGB_2101010,
	D3D_AWVU_2101010,

	//16 bits per channel
	D3D_GR_1616,
	D3D_VU_1616,
	D3D_ABGR_16161616,

	//HDR formats
	D3D_R16F,
	D3D_GR_1616F,
	D3D_ABGR_16161616F,

	//32 bits per channel
	D3D_R32F,
	D3D_GR_3232F,
	D3D_ABGR_32323232F,

	// Ericsson
	ETC_RGB_4BPP,
	ETC_RGBA_EXPLICIT,
	ETC_RGBA_INTERPOLATED,

	MGLPT_NOTYPE = 0xff

} PixelType;

/*****************************************************************************
 * constants
 *****************************************************************************/

const unsigned int PVRTEX_MIPMAP		= (1<<8);		// has mip map levels
const unsigned int PVRTEX_TWIDDLE		= (1<<9);		// is twiddled
const unsigned int PVRTEX_BUMPMAP		= (1<<10);		// has normals encoded for a bump map
const unsigned int PVRTEX_TILING		= (1<<11);		// is bordered for tiled pvr
const unsigned int PVRTEX_CUBEMAP		= (1<<12);		// is a cubemap/skybox
const unsigned int PVRTEX_FALSEMIPCOL	= (1<<13);		//
const unsigned int PVRTEX_VOLUME		= (1<<14);
const unsigned int PVRTEX_PIXELTYPE		= 0xff;			// pixel type is always in the last 16bits of the flags
const unsigned int PVRTEX_IDENTIFIER	= 0x21525650;	// the pvr identifier is the characters 'P','V','R'

const unsigned int PVRTEX_V1_HEADER_SIZE = 44;			// old header size was 44 for identification purposes

const unsigned int PVRTC2_MIN_TEXWIDTH		= 16;
const unsigned int PVRTC2_MIN_TEXHEIGHT		= 8;
const unsigned int PVRTC4_MIN_TEXWIDTH		= 8;
const unsigned int PVRTC4_MIN_TEXHEIGHT		= 8;
const unsigned int ETC_MIN_TEXWIDTH			= 4;
const unsigned int ETC_MIN_TEXHEIGHT		= 4;

/****************************************************************************
** Functions
****************************************************************************/

/*!***************************************************************************
@Function		PVRTTextureCreate
@Input			w			Size of the texture
@Input			h			Size of the texture
@Input			wMin		Minimum size of a texture level
@Input			hMin		Minimum size of a texture level
@Input			nBPP		Bits per pixel of the format
@Input			bMIPMap		Create memory for MIP-map levels also?
@Return			Allocated texture memory (must be free()d)
@Description	Creates a PVR_Texture_Header structure, including room for
				the specified texture, in memory.
*****************************************************************************/
PVR_Texture_Header *PVRTTextureCreate(
	unsigned int		w,
	unsigned int		h,
	const unsigned int	wMin,
	const unsigned int	hMin,
	const unsigned int	nBPP,
	const bool			bMIPMap);

/*!***************************************************************************
@Function		PVRTTextureTile
@Modified		pOut		The tiled texture in system memory
@Input			pIn			The source texture
@Input			nRepeatCnt	Number of times to repeat the source texture
@Description	Allocates and fills, in system memory, a texture large enough
				to repeat the source texture specified number of times.
*****************************************************************************/
void PVRTTextureTile(
	PVR_Texture_Header			**pOut,
	const PVR_Texture_Header	* const pIn,
	const int					nRepeatCnt);

/****************************************************************************
** Internal Functions
****************************************************************************/

/*!***************************************************************************
@Function		PVRTTextureLoadTiled
@Description	Needed by PVRTTextureTile() in the various PVRTTextureAPIs
*****************************************************************************/
void PVRTTextureLoadTiled(
	unsigned char		* const pDst,
	const unsigned int	nWidthDst,
	const unsigned int	nHeightDst,
	const unsigned char	* const pSrc,
	const unsigned int	nWidthSrc,
	const unsigned int	nHeightSrc,
	const unsigned int	nElementSize,		// Bytes per pixel
	const bool			bTwiddled);


#endif /* _PVRTTEXTURE_H_ */


/*****************************************************************************
 End of file (PVRTTexture.h)
*****************************************************************************/
