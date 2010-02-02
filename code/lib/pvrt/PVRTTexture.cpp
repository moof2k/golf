/******************************************************************************

 @File         PVRTTexture.cpp

 @Title        

 @Copyright    Copyright (C) 2004 - 2008 by Imagination Technologies Limited.

 @Platform     ANSI compatible

 @Description  Texture loading.

******************************************************************************/
#include <string.h>
#include <stdlib.h>

#include "PVRTGlobal.h"
//#include "PVRTContext.h"
#include "PVRTTexture.h"
#include "PVRTFixedPoint.h"
#include "PVRTMatrix.h"
#include "PVRTMisc.h"

/*****************************************************************************
** Functions
*****************************************************************************/

void PVRTTextureLoadTiled(
	unsigned char		* const pDst,
	const unsigned int	nWidthDst,
	const unsigned int	nHeightDst,
	const unsigned char	* const pSrc,
	const unsigned int	nWidthSrc,
	const unsigned int	nHeightSrc,
	const unsigned int	nElementSize,		// Bytes per pixel
	const bool			bTwiddled)
{
	unsigned int nXs, nYs;
	unsigned int nXd, nYd;
	unsigned int nIdxSrc, nIdxDst;

	for(nIdxDst = 0; nIdxDst < nWidthDst*nHeightDst; ++nIdxDst)
	{
		if(bTwiddled)
		{
			PVRTMiscDeTwiddle(nXd, nYd, nIdxDst);
		}
		else
		{
			nXd = nIdxDst % nWidthDst;
			nYd = nIdxDst / nWidthDst;
		}

		nXs = nXd % nWidthSrc;
		nYs = nYd % nHeightSrc;

		if(bTwiddled)
		{
			PVRTMiscTwiddle(nIdxSrc, nXs, nYs);
		}
		else
		{
			nIdxSrc = nYs * nWidthSrc + nXs;
		}

		memcpy(pDst + nIdxDst*nElementSize, pSrc + nIdxSrc*nElementSize, nElementSize);
	}
}

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
	const bool			bMIPMap)
{
	size_t			len;
	unsigned char	*p;

	len = 0;
	do
	{
		len += PVRT_MAX(w, wMin) * PVRT_MAX(h, hMin);
		w >>= 1;
		h >>= 1;
	}
	while(bMIPMap && (w || h));

	len = (len * nBPP) / 8;
	len += sizeof(PVR_Texture_Header);

	p = (unsigned char*)malloc(len);
	_ASSERT(p);
	return (PVR_Texture_Header*)p;
}

/*****************************************************************************
 End of file (PVRTTexture.cpp)
*****************************************************************************/
