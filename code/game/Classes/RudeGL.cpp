
#include "RudeMath.h"
#include "RudeGL.h"

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>

RudeGL RGL;

#define DEBUG 0



RudeGL::RudeGL()
: m_eye(0,0,0)
{
	for(int i = 0; i < kNumRudeGLEnableOptions; i++)
		m_enables[i] = false;
}

RudeGL::~RudeGL()
{
}


void RudeGL::SetViewport(int top, int left, int bottom, int right)
{
	m_viewport.m_top = top;
	m_viewport.m_left = left;
	m_viewport.m_bottom = bottom;
	m_viewport.m_right = right;
	float screenx = right - left;
	float screeny = bottom - top;
	
	glViewport(m_viewport.m_left, 480 - m_viewport.m_bottom, screenx, screeny);
}


void RudeGL::Ortho(float ox, float oy, float oz, float w, float h, float d)
{	
	//float ww = w / 2.0f;
	//float wh = h / 2.0f;
	//wd = d / 2.0f;
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glOrthof(ox, ox + w, oy, oy + h, oz, oz + d);
	//glTranslatef(ww, wh, 0.0f);
	glScalef(1.0f, -1.0f, 1.0f);
	glTranslatef(0.0f, -h, 0.0f);
	
	
}

void RudeGL::Frustum(float ox, float oy, float w, float h, float near, float far)
{
	float ww = w / 2.0f;
	float wh = h / 2.0f;
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	

	glFrustumf(ox - ww, ox + ww, oy - wh, oy + wh, near, far);
}

void CrossProd(float x1, float y1, float z1, float x2, float y2, float z2, float res[3]) 
{ 
	res[0] = y1*z2 - y2*z1; 
	res[1] = x2*z1 - x1*z2; 
	res[2] = x1*y2 - x2*y1; 
} 

void RudeGL::LookAt(float eyeX, float eyeY, float eyeZ, float lookAtX, float lookAtY, float lookAtZ, float upX, float upY, float upZ)
{
	
	float f[3]; 
	
	// calculating the viewing vector 
	f[0] = lookAtX - eyeX; 
	f[1] = lookAtY - eyeY; 
	f[2] = lookAtZ - eyeZ; 
	
	float fMag, upMag; 
	fMag = sqrt(f[0]*f[0] + f[1]*f[1] + f[2]*f[2]); 
	upMag = sqrt(upX*upX + upY*upY + upZ*upZ); 
	
	// normalizing the viewing vector 
	if( fMag != 0) 
	{ 
		f[0] = f[0]/fMag; 
		f[1] = f[1]/fMag; 
		f[2] = f[2]/fMag; 
	} 
	
	// normalising the up vector. no need for this here if you have your 
	// up vector already normalised, which is mostly the case. 
	if( upMag != 0 ) 
	{ 
		upX = upX/upMag; 
		upY = upY/upMag; 
		upZ = upZ/upMag; 
	} 
	
	float s[3], u[3]; 
	
	CrossProd(f[0], f[1], f[2], upX, upY, upZ, s); 
	CrossProd(s[0], s[1], s[2], f[0], f[1], f[2], u); 
	
	float M[]= 
	{ 
		s[0], u[0], -f[0], 0, 
		s[1], u[1], -f[1], 0, 
		s[2], u[2], -f[2], 0, 
		0, 0, 0, 1 
	}; 
	
	glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	glMultMatrixf(M); 
	glTranslatef (-eyeX, -eyeY, -eyeZ);
	
	m_eye = btVector3(eyeX, eyeY, eyeZ);

}

void RudeGL::LoadIdentity()
{	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void RudeGL::Translate(float x, float y, float z)
{	
	glMatrixMode(GL_MODELVIEW);
	glTranslatef(x, y, z);
}

void RudeGL::TranslateView(float x, float y, float z)
{
	glMatrixMode(GL_PROJECTION);
	glTranslatef(x, y, z);
}

void RudeGL::Scale(float sx, float sy, float sz)
{
	glMatrixMode(GL_MODELVIEW);
	glScalef(sx, sy, sz);
}

void RudeGL::Rotate(float degrees, float ax, float ay, float az)
{	
	glMatrixMode(GL_MODELVIEW);
	glRotatef(degrees, ax, ay, az);
}

void RudeGL::RotateX(int degrees)
{
	Rotate(degrees, 1.0f, 0.0f, 0.0f);
}

void RudeGL::RotateY(int degrees)
{
	Rotate(degrees, 0.0f, 1.0f, 0.0f);
}


void RudeGL::RotateZ(int degrees)
{
	Rotate(degrees, 0.0f, 0.0f, 1.0f);
}


void RudeGL::RotateView(float degrees, float ax, float ay, float az)
{	
	glMatrixMode(GL_PROJECTION);
	glRotatef(degrees, ax, ay, az); 

}


btVector3 RudeGL::Project(const btVector3 &point)
{
	float hw = (m_viewport.m_right - m_viewport.m_left) / 2.0f;
	float hh = (m_viewport.m_bottom - m_viewport.m_top) / 2.0f;
	
	float pmat[16];
	glGetFloatv(GL_PROJECTION_MATRIX, pmat);
	
	float result[4];
	
	for(int y = 0; y < 4; y++)
	{
		result[y] = point.x() * pmat[0 * 4 + y]
				  + point.y() * pmat[1 * 4 + y]
				  + point.z() * pmat[2 * 4 + y]
		          + 1.0f      * pmat[3 * 4 + y];
		
	}
	
	btVector3 rv;
	rv.setX((result[0] / result[3] * hw) + hw);
	rv.setY((2.0f * hh) - ((result[1] / result[3] * hh) + hh));
	rv.setZ(result[2] / result[3]);
	
	
	
	return rv;
}

void RudeGL::Enable(eRudeGLEnableOption option, bool enable)
{
	if(m_enables[option] == enable)
		return;
	
	m_enables[option] = enable;
	
	switch(option)
	{
		case kDepthTest:
			if(enable)
				glEnable(GL_DEPTH_TEST);
			else
				glDisable(GL_DEPTH_TEST);
			break;
		case kBackfaceCull:
			if(enable)
				glEnable(GL_CULL_FACE);
			else
				glDisable(GL_CULL_FACE);
			break;
			
		default:
			RUDE_ASSERT(0, "Unhandled option");
			break;
	}
}

