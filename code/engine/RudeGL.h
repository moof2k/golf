/*
 *  RudeGL.h
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */


#ifndef __H_RudeGL_OGLES
#define __H_RudeGL_OGLES

#include "Rude.h"
#include "RudeRect.h"


#define kNumberPi 3.14159267f
#define kNumberInversePi .318309886184f
#define kNumberInverse180 0.00555555555f
#define kNumberPiTimesInverse180 0.01745329252f
#define kNumberSquareroot2 1.41421356237f
#define kNumber180OverPi 57.2957792137960f

#include <btBulletDynamicsCommon.h>


typedef enum {
	kDepthTest = 0,
	kBackfaceCull,
	
	kNumRudeGLEnableOptions
} eRudeGLEnableOption;

typedef enum {
	kVertexArray,
	kColorArray,
	kTextureCoordArray,
	
	kNumRudeGLEnableClientOptions
} eRudeGLEnableClientOption;

class RudeGL
{
public:

	RudeGL();
	~RudeGL(void);

	void SetViewport(int top, int left, int bottom, int right);
	void Ortho(float ox, float oy, float oz, float w, float h, float d);
	void Frustum(float ox, float oy, float w, float h, float near, float far);
	void LookAt(float eyex, float eyey, float eyez, float vx, float vy, float vz,
			float upx, float upy, float upz);
	void LoadIdentity();
	void Translate(float x, float y, float z);
	void Scale(float sx, float sy, float sz);
	void Rotate(float degrees, float ax, float ay, float az);
	void RotateX(int degrees);
	void RotateY(int degrees);
	void RotateZ(int degrees);
	void RotateView(float degrees, float ax, float ay, float az);
	void TranslateView(float x, float y, float z);
	
	btVector3 Project(const btVector3 &point);
	btVector3 InverseProject(const btVector3 &point);
	
	void Enable(eRudeGLEnableOption option, bool enable);
	void EnableClient(eRudeGLEnableClientOption option, bool enable);
	void FlushEnables();

	btVector3 GetEye() { return m_eye; }
	btVector3 GetLookAt() { return m_lookAt; }
	btVector3 GetForward() { return m_forward; }
	btVector3 GetUp() { return m_up; }
	
	float GetHalfWidth() { return m_hw; }
	float GetHalfHeight() { return m_hh; }
	
	float * GetViewMat() { return m_viewmat; }
	
	void SetLandscape(bool landscape) { m_landscape = landscape; }
	bool GetLandscape() { return m_landscape; }
	
	void SetUpsideDown(bool upsideDown) { m_upsideDown = upsideDown; }
	bool GetUpsideDown() { return m_upsideDown; }

	void SetDeviceHeight(float h) { m_deviceHeight = h; }
	float GetDeviceHeight() { return m_deviceHeight; }

	void SetDeviceWidth(float w) { m_deviceWidth = w; }
	float GetDeviceWidth() { return m_deviceWidth; }

private:
	
	float m_viewmat[16];

	bool m_enables[kNumRudeGLEnableOptions];
	bool m_enableClients[kNumRudeGLEnableClientOptions];
	
	RudeRect m_viewport;				// screen viewport rect
	btVector3 m_eye;
	btVector3 m_lookAt;
	btVector3 m_forward;
	btVector3 m_up;
	float m_hw;
	float m_hh;
	float m_near;
	
	bool m_landscape;
	bool m_upsideDown;
	float m_deviceHeight;
	float m_deviceWidth;
	
};

extern RudeGL RGL;

#endif
