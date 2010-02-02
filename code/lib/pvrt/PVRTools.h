/******************************************************************************

 @File         PVRTools.h

 @Title        

 @Copyright    Copyright (C) 2003 - 2008 by Imagination Technologies Limited.

 @Platform     ANSI compatible

 @Description  Header file of OGLES2Tools.lib.

******************************************************************************/
#ifndef PVRTOOLS_H
#define PVRTOOLS_H

#ifdef BUILD_OGLES2
	#include "OGLES2Tools.h"
#elif BUILD_OGLES
	#include "OGLESTools.h"
#elif BUILD_OGL
	#include "OGLTools.h"
#elif BUILD_D3DM
	#include "D3DMTools.h"
#elif BUILD_DX9
	#include "DX9Tools.h"
#endif

//#include "PVRTContext.h"
//#include "PVRTContextAPI.h"
//#include "PVRTFixedPoint.h"
//#include "PVRTMatrix.h"
//#include "PVRTQuaternion.h"
//#include "PVRTTrans.h"
//#include "PVRTVertex.h"
//#include "PVRTMisc.h"
////#include "PVRTMiscBackground.h"
//#include "PVRTPrint3D.h"
//#include "PVRTBoneBatch.h"
//#include "PVRTModelPOD.h"
//
//#include "PVRTTexture.h"
//#include "PVRTTriStrip.h"
//#include "PVRTPFXParser.h"
//#include "PVRTShadowVol.h"

#endif /* PVRTOOLS_H*/

/*****************************************************************************
 End of file (Tools.h)
*****************************************************************************/
