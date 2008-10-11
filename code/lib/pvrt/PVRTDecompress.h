/******************************************************************************

 @File         PVRTDecompress.h

 @Title        

 @Copyright    Copyright (C) 2000 - 2008 by Imagination Technologies Limited.

 @Platform     ANSI compatible

 @Description  PVRTC Texture Decompression.

******************************************************************************/
#ifndef PVRTDECOMPRESS_H
#define PVRTDECOMPRESS_H

//#ifdef __cplusplus
//extern "C" {
//#endif

void PVRTCDecompress(const void *pCompressedData,
				const int Do2bitMode,
				const int XDim,
				const int YDim,
				unsigned char* pResultImage);
				
//#ifdef __cplusplus
//}
//#endif

int ETCDecompress(const void * const pSrcData,
						 const unsigned int &x,
						 const unsigned int &y,
						 void *pDestData,
						 const int &nMode);


#endif //PVRTDECOMPRESS_H

/*
// END OF FILE
*/

