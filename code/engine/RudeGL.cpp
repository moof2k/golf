/*
 *  RudeGL.cpp
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#include "RudeMath.h"
#include "RudeGL.h"

RudeGL RGL;

#define DEBUG 0


const unsigned int kRudeEnableMappings[kNumRudeGLEnableOptions] = {
	GL_DEPTH_TEST,
	GL_CULL_FACE,
};


const unsigned int kRudeEnableClientMappings[kNumRudeGLEnableClientOptions] = {
	GL_VERTEX_ARRAY,
	GL_COLOR_ARRAY,
	GL_TEXTURE_COORD_ARRAY
};

RudeGL::RudeGL()
: m_eye(0,0,0)
, m_lookAt(0,0,1)
, m_forward(0,0,1)
, m_landscape(false)
, m_upsideDown(false)
, m_deviceHeight(480.0f)
, m_deviceWidth(320.0f)
{
	for(int i = 0; i < kNumRudeGLEnableOptions; i++)
		m_enables[i] = false;
	
	for(int i = 0; i < kNumRudeGLEnableClientOptions; i++)
		m_enableClients[i] = false;
}

RudeGL::~RudeGL()
{
}

/**
 * Sets the viewport
 */
void RudeGL::SetViewport(int top, int left, int bottom, int right)
{

#ifdef RUDE_IPHONE
	m_viewport.m_top = top;
	m_viewport.m_left = left;
	m_viewport.m_bottom = bottom;
	m_viewport.m_right = right;
	float screenx = right - left;
	float screeny = bottom - top;
	

	if(m_landscape)
	{
		if(m_upsideDown)
			glViewport(m_deviceWidth - m_viewport.m_bottom, m_viewport.m_left, screeny, screenx);
		else
			glViewport(m_deviceWidth - m_viewport.m_bottom, m_viewport.m_left, screeny, screenx);
	}
	else
	{
		if(m_upsideDown)
			glViewport(m_deviceWidth - m_viewport.m_right, m_viewport.m_top, screenx, screeny);
		else
			glViewport(m_viewport.m_left, m_deviceHeight - m_viewport.m_bottom, screenx, screeny);
	}
#else
	
	m_viewport.m_top = top;
	m_viewport.m_left = left;
	m_viewport.m_bottom = bottom;
	m_viewport.m_right = right;
	float screenx = right - left;
	float screeny = bottom - top;
	
	glViewport(m_viewport.m_left, m_deviceHeight - m_viewport.m_bottom, screenx, screeny);
#endif
}

/**
 * Sets the projection matrix to use an orthographic transform
 */
void RudeGL::Ortho(float ox, float oy, float oz, float w, float h, float d)
{	
	glCullFace(GL_BACK);
	
	//float ww = w / 2.0f;
	//float wh = h / 2.0f;
	//wd = d / 2.0f;
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	if(m_landscape)
	{
		if(!m_upsideDown)
			glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
		else
			glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	}
	else
	{
		if(m_upsideDown)
			glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
	}

#ifdef RUDE_OGLES
	glOrthof(ox, ox + w, oy, oy + h, oz, oz + d);
#else
	glOrtho(ox, ox + w, oy, oy + h, oz, oz + d);
#endif
    
	//glTranslatef(ww, wh, 0.0f);
	glScalef(1.0f, -1.0f, 1.0f);
	glTranslatef(0.0f, -h, 0.0f);
	
	
}

/**
 * Sets the view frustum for the projection matrix
 */
void RudeGL::Frustum(float ox, float oy, float w, float h, float near_plane, float far_plane)
{
	m_hw = w / 2.0f;
	m_hh = h / 2.0f;
	m_near = near_plane;
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	if(m_landscape)
	{
		if(!m_upsideDown)
			glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
		else
			glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	}
	else
	{
		if(m_upsideDown)
			glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
	}

#ifdef RUDE_OGLES
	glFrustumf(ox - m_hw, ox + m_hw, oy - m_hh, oy + m_hh, near_plane, far_plane);
#else
	glFrustum(ox - m_hw, ox + m_hw, oy - m_hh, oy + m_hh, near_plane, far_plane);
#endif
}

void CrossProd(float x1, float y1, float z1, float x2, float y2, float z2, float res[3]) 
{ 
	res[0] = y1*z2 - y2*z1; 
	res[1] = x2*z1 - x1*z2; 
	res[2] = x1*y2 - x2*y1; 
} 

/**
 * Builds a "lookat" matrix and applies it to the projection matrix
 */
void RudeGL::LookAt(float eyeX, float eyeY, float eyeZ, float lookAtX, float lookAtY, float lookAtZ, float upX, float upY, float upZ)
{
	
	m_eye = btVector3(eyeX, eyeY, eyeZ);
	m_lookAt = btVector3(lookAtX, lookAtY, lookAtZ);
	
	btVector3 inup(upX, upY, upZ);
	
	m_forward = m_eye - m_lookAt;
	m_forward.normalize();
	
	btVector3 side = inup.cross(m_forward);
	side.normalize();
	
	m_up = m_forward.cross(side);
	
	
	float M[]= 
	{ 
		side.x(), m_up.x(), m_forward.x(), 0, 
		side.y(), m_up.y(), m_forward.y(), 0, 
		side.z(), m_up.z(), m_forward.z(), 0, 
		0, 0, 0, 1 
	};
	
	glMatrixMode(GL_PROJECTION);
	glMultMatrixf(M); 
	glTranslatef (-eyeX, -eyeY, -eyeZ);
	
	
}

/**
 * Reset the model matrix to the identity matrix
 */
void RudeGL::LoadIdentity()
{	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


/**
 * Translate the model matrix
 */
void RudeGL::Translate(float x, float y, float z)
{	
	glMatrixMode(GL_MODELVIEW);
	glTranslatef(x, y, z);
}

/**
 * Translate the projection matrix
 */
void RudeGL::TranslateView(float x, float y, float z)
{
	glMatrixMode(GL_PROJECTION);
	glTranslatef(x, y, z);
}

/**
 * Scale the model matrix
 */
void RudeGL::Scale(float sx, float sy, float sz)
{
	glMatrixMode(GL_MODELVIEW);
	glScalef(sx, sy, sz);
}

/**
 * Rotate the model matrix by 'degrees' about an arbitrary axis
 */
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

/**
 * Rotates the projection matrix
 */
void RudeGL::RotateView(float degrees, float ax, float ay, float az)
{	
	glMatrixMode(GL_PROJECTION);
	glRotatef(degrees, ax, ay, az); 

}

/**
 * Project a point in world coordinates to screen coordinates
 */
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
	
	if(m_landscape == false)
	{
		if(m_upsideDown == false)
		{
			rv.setX((result[0] / result[3] * hw) + hw);
			rv.setY((2.0f * hh) - ((result[1] / result[3] * hh) + hh));
			rv.setZ(result[2] / result[3]);
		}
		else
		{
			rv.setX((2.0f * hw) - ((result[0] / result[3] * hw) + hw));
			rv.setY((result[1] / result[3] * hh) + hh);
			rv.setZ(result[2] / result[3]);
		}
	}
	else
	{
		rv.setX((2.0f * hw) - ((result[1] / result[3] * hw) + hw));
		rv.setY((2.0f * hh) - ((result[0] / result[3] * hh) + hh));
		rv.setZ(result[2] / result[3]);
	}

	rv += btVector3(0.5f, 0.5f, 0.0f);
	
	return rv;
}

static void __gluMakeIdentityf(GLfloat m[16])
{
    m[0+4*0] = 1; m[0+4*1] = 0; m[0+4*2] = 0; m[0+4*3] = 0;
    m[1+4*0] = 0; m[1+4*1] = 1; m[1+4*2] = 0; m[1+4*3] = 0;
    m[2+4*0] = 0; m[2+4*1] = 0; m[2+4*2] = 1; m[2+4*3] = 0;
    m[3+4*0] = 0; m[3+4*1] = 0; m[3+4*2] = 0; m[3+4*3] = 1;
}

/*
 ** inverse = invert(src)
 */
static int __gluInvertMatrixd(const GLfloat src[16], GLfloat inverse[16])
{
    int i, j, k, swap;
    float t;
    GLfloat temp[4][4];
	
    for (i=0; i<4; i++) {
		for (j=0; j<4; j++) {
			temp[i][j] = src[i*4+j];
		}
    }
    __gluMakeIdentityf(inverse);
	
    for (i = 0; i < 4; i++) {
		/*
		 ** Look for largest element in column
		 */
		swap = i;
		for (j = i + 1; j < 4; j++) {
			if (fabs(temp[j][i]) > fabs(temp[i][i])) {
				swap = j;
			}
		}
		
		if (swap != i) {
			/*
			 ** Swap rows.
			 */
			for (k = 0; k < 4; k++) {
				t = temp[i][k];
				temp[i][k] = temp[swap][k];
				temp[swap][k] = t;
				
				t = inverse[i*4+k];
				inverse[i*4+k] = inverse[swap*4+k];
				inverse[swap*4+k] = t;
			}
		}
		
		if (temp[i][i] == 0) {
			/*
			 ** No non-zero pivot.  The matrix is singular, which shouldn't
			 ** happen.  This means the user gave us a bad matrix.
			 */
			return GL_FALSE;
		}
		
		t = temp[i][i];
		for (k = 0; k < 4; k++) {
			temp[i][k] /= t;
			inverse[i*4+k] /= t;
		}
		for (j = 0; j < 4; j++) {
			if (j != i) {
				t = temp[j][i];
				for (k = 0; k < 4; k++) {
					temp[j][k] -= temp[i][k]*t;
					inverse[j*4+k] -= inverse[i*4+k]*t;
				}
			}
		}
    }
    return GL_TRUE;
}

static void __gluMultMatrixVecd(const GLfloat matrix[16], const GLfloat in[4],
								GLfloat out[4])
{
    int i;
	
    for (i=0; i<4; i++) {
		out[i] = 
	    in[0] * matrix[0*4+i] +
	    in[1] * matrix[1*4+i] +
	    in[2] * matrix[2*4+i] +
	    in[3] * matrix[3*4+i];
    }
}

/**
 * Project a screen coordinate to a point on the near plane.
 * This method assumes the viewport is set to the full screen,
 * and won't work if the viewport is anything else.
 */
btVector3 RudeGL::InverseProject(const btVector3 &point)
{
	float finalMatrix[16];
	float projMatrix[16];
    float in[4];
    float out[4];
	
	glGetFloatv(GL_PROJECTION_MATRIX, projMatrix);
	__gluInvertMatrixd(projMatrix, finalMatrix);
	
	if(m_landscape)
	{
		in[0] = m_deviceWidth - point.y();
		in[1] = point.x();
	}
	else
	{
		if(m_upsideDown)
		{
			in[0] = m_deviceWidth - point.x();
			in[1] = m_deviceHeight - point.y();
		}
		else
		{
			in[0] = point.x();
			in[1] = point.y();
		}
	}
	
    in[2] = 1.0f;
    in[3] = 1.0f;
	
	
    /* Map x and y from window coordinates */
    in[0] = (in[0] - 0.0f) / m_deviceWidth;
    in[1] = (in[1] - 0.0f) / m_deviceHeight;
	
    /* Map to range -1 to 1 */
    in[0] = in[0] * 2 - 1;
    in[1] = -(in[1] * 2 - 1);
    in[2] = in[2] * 2 - 1;
	
    __gluMultMatrixVecd(finalMatrix, in, out);
	//printf("in: %f %f out: %f %f %f\n", p.x(), p.y(), out[0], out[1], out[2]);
	out[0] /= out[3];
    out[1] /= out[3];
    out[2] /= out[3];
	
	btVector3 rv(out[0], out[1], out[2]);
	
	return rv;
	
}

/**
 * Set the given OpenGL attribute on or off.  Using this function
 * instead of calling glEnable() directly prevents unnecessary
 * draw calls.
 */
void RudeGL::Enable(eRudeGLEnableOption option, bool enable)
{
	if(m_enables[option] == enable)
		return;
	
	m_enables[option] = enable;
	
	
	if(enable)
		glEnable(kRudeEnableMappings[option]);
	else
		glDisable(kRudeEnableMappings[option]);

	

}

/**
 * Set the given OpenGL client state on or off.  Using this function
 * instead of calling glEnableClientState() directly prevents unnecessary
 * draw calls.
 */
void RudeGL::EnableClient(eRudeGLEnableClientOption option, bool enable)
{
	if(m_enableClients[option] == enable)
		return;
	
	m_enableClients[option] = enable;
	
	if(enable)
		glEnableClientState(kRudeEnableClientMappings[option]);
	else
		glDisableClientState(kRudeEnableClientMappings[option]);
		
}

/**
 * When switching to fullscreen mode on MacOS we need to flush the cached
 * state of the enables because it's lost when going fullscreen
 */
void RudeGL::FlushEnables()
{
	for(int i = 0; i < kNumRudeGLEnableOptions; i++)
	{
		m_enables[i] = false;
		glDisable(kRudeEnableMappings[i]);
	}

	for(int i = 0; i < kNumRudeGLEnableClientOptions; i++)
	{
		m_enableClients[i] = false;
		glDisableClientState(kRudeEnableClientMappings[i]);
	}
}

