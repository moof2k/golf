/******************************************************************************

 @File         PVRTShadowVol.h

 @Title        

 @Copyright    Copyright (C) 2001 - 2008 by Imagination Technologies Limited.

 @Platform     ANSI compatible

 @Description  Declarations of functions relating to shadow volume generation.

******************************************************************************/
#ifndef _PVRTSHADOWVOL_H_
#define _PVRTSHADOWVOL_H_

#include "PVRTContext.h"
#include "PVRTVector.h"

/****************************************************************************
** Defines
****************************************************************************/
#define PVRTSHADOWVOLUME_VISIBLE		0x00000001
#define PVRTSHADOWVOLUME_NEED_CAP_FRONT	0x00000002
#define PVRTSHADOWVOLUME_NEED_CAP_BACK	0x00000004
#define PVRTSHADOWVOLUME_NEED_ZFAIL		0x00000008

/****************************************************************************
** Structures
****************************************************************************/
struct PVRTSvMEdge {
	unsigned short		wV0, wV1;		/*!< Indices of the vertices of the edge */
	int			nVis;			/*!< Bit0 = Visible, Bit1 = Hidden, Bit2 = Reverse Winding */
};

struct PVRTSvMTriangle {
	unsigned short		w[3];				/*!< Source indices of the triangle */
	PVRTSvMEdge	*pE0, *pE1, *pE2;	/*!< Edges of the triangle */
	PVRTVECTOR3	vNormal;			/*!< Triangle normal */
	int			nWinding;			/*!< BitN = Correct winding for edge N */
};

struct PVRTSvShadowMesh {
	PVRTVECTOR3		*pV;	/*!< Unique vertices in object space */
	PVRTSvMEdge		*pE;
	PVRTSvMTriangle	*pT;
	unsigned int	nV;		/*!< Vertex count */
	unsigned int	nE;		/*!< Edge count */
	unsigned int	nT;		/*!< Triangle count */

#ifdef BUILD_DX9
	IDirect3DVertexBuffer9	*pivb;		/*!< Two copies of the vertices */
#endif
#if defined(BUILD_OGL) || defined(BUILD_OGLES) || defined(BUILD_OGLES2)
	void			*pivb;		/*!< Two copies of the vertices */
#endif
};

/*
	Renderable shadow-volume information:
*/
struct PVRTSvShadowVol {
#ifdef BUILD_DX9
	IDirect3DIndexBuffer9	*piib;		/*!< Indices to render the volume */
#endif
#if defined(BUILD_OGL) || defined(BUILD_OGLES) || defined(BUILD_OGLES2)
	unsigned short					*piib;		/*!< Indices to render the volume */
#endif
	unsigned int			nIdxCnt;	/*!< Number of indices in piib */

#ifdef _DEBUG
	unsigned int			nIdxCntMax;	/*!< Number of indices which can fit in piib */
#endif
};

/****************************************************************************
** Declarations
****************************************************************************/
void PVRTSvMeshCreateMesh(
	PVRTSvShadowMesh		* const psMesh,
	const float				* const pVertex,
	const unsigned int		nNumVertex,
	const unsigned short	* const pFaces,
	const unsigned int		nNumFaces);

bool PVRTSvMeshInitMesh(
	PVRTSvShadowMesh		* const psMesh,
	const SPVRTContext		* const pContext);

bool PVRTSvMeshInitVol(
	PVRTSvShadowVol			* const psVol,
	const PVRTSvShadowMesh	* const psMesh,
	const SPVRTContext		* const pContext);

void PVRTSvMeshDestroyMesh(
	PVRTSvShadowMesh		* const psMesh);

void PVRTSvMeshReleaseMesh(
	PVRTSvShadowMesh		* const psMesh);

void PVRTSvMeshReleaseVol(
	PVRTSvShadowVol			* const psVol);

void PVRTSvSilhouetteProjectedBuild(
	PVRTSvShadowVol			* const psVol,
	const unsigned int				dwVisFlags,
	const PVRTSvShadowMesh	* const psMesh,
	const PVRTVECTOR3		* const pvLightModel,
	const bool				bPointLight);

void PVRTSvSilhouetteProjectedBuild(
	PVRTSvShadowVol			* const psVol,
	const unsigned int		dwVisFlags,
	const PVRTSvShadowMesh	* const psMesh,
	const PVRT::Vec3		* const pvLightModel,
	const bool				bPointLight);

void PVRTSvBoundingBoxExtrude(
	PVRTVECTOR3				* const pvExtrudedCube,
	const PVRTBOUNDINGBOX	* const pBoundingBox,
	const PVRTVECTOR3		* const pvLightMdl,
	const bool				bPointLight,
	const float				fVolLength);

void PVRTSvBoundingBoxIsVisible(
	unsigned int			* const pdwVisFlags,
	const bool				bObVisible,				// Is the object visible?
	const bool				bNeedsZClipping,		// Does the object require Z clipping?
	const PVRTBOUNDINGBOX	* const pBoundingBox,
	const PVRTMATRIX		* const pmTrans,
	const PVRTVECTOR3		* const pvLightMdl,
	const bool				bPointLight,
	const float				fCamZProj,
	const float				fVolLength);

int PVRTSvSilhouetteProjectedRender(
	const PVRTSvShadowMesh	* const psMesh,
	const PVRTSvShadowVol	* const psVol,
	const SPVRTContext		* const pContext);


#endif /* _PVRTSHADOWVOL_H_ */

/*****************************************************************************
 End of file (PVRTShadowVol.h)
*****************************************************************************/
