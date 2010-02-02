/******************************************************************************

 @File         PVRTVector.cpp

 @Title        

 @Copyright    Copyright (C) 2007 - 2008 by Imagination Technologies Limited.

 @Platform     ANSI compatible

 @Description  Vector and matrix mathematics library

******************************************************************************/
#include "PVRTVector.h"

#include <math.h>

namespace PVRT
{
/*!***************************************************************************
** Vec3 3 component vector
****************************************************************************/

/*!***************************************************************************
 @Function			Vec3
 @Input				v4Vec a Vec4
 @Description		Constructor from a Vec4
*****************************************************************************/
	Vec3::Vec3(const Vec4& vec4)
	{
		x = vec4.x; y = vec4.y; z = vec4.z;
	}

/*!***************************************************************************
 @Function			*
 @Input				rhs a Mat3
 @Returns			result of multiplication
 @Description		matrix multiplication operator Vec3 and Mat3
****************************************************************************/
	Vec3 Vec3::operator*(const Mat3& rhs) const
	{
		Vec3 out;

		out.x = VERTTYPEMUL(x,rhs.f[0])+VERTTYPEMUL(y,rhs.f[1])+VERTTYPEMUL(z,rhs.f[2]);
		out.y = VERTTYPEMUL(x,rhs.f[3])+VERTTYPEMUL(y,rhs.f[4])+VERTTYPEMUL(z,rhs.f[5]);
		out.z = VERTTYPEMUL(x,rhs.f[6])+VERTTYPEMUL(y,rhs.f[7])+VERTTYPEMUL(z,rhs.f[8]);

		return out;
	}

/*!***************************************************************************
 @Function			*=
 @Input				rhs a Mat3
 @Returns			result of multiplication and assignment
 @Description		matrix multiplication and assignment operator for Vec3 and Mat3
****************************************************************************/
	Vec3& Vec3::operator*=(const Mat3& rhs)
	{
		VERTTYPE tx = VERTTYPEMUL(x,rhs.f[0])+VERTTYPEMUL(y,rhs.f[1])+VERTTYPEMUL(z,rhs.f[2]);
		VERTTYPE ty = VERTTYPEMUL(x,rhs.f[3])+VERTTYPEMUL(y,rhs.f[4])+VERTTYPEMUL(z,rhs.f[5]);
		z = VERTTYPEMUL(x,rhs.f[6])+VERTTYPEMUL(y,rhs.f[7])+VERTTYPEMUL(z,rhs.f[8]);
		x = tx;
		y = ty;

		return *this;
	}

/*!***************************************************************************
** Vec4 4 component vector
****************************************************************************/

/*!***************************************************************************
 @Function			*
 @Input				rhs a Mat4
 @Returns			result of multiplication
 @Description		matrix multiplication operator Vec4 and Mat4
****************************************************************************/
	Vec4 Vec4::operator*(const Mat4& rhs) const
	{
		Vec4 out;
		out.x = VERTTYPEMUL(x,rhs.f[0])+VERTTYPEMUL(y,rhs.f[1])+VERTTYPEMUL(z,rhs.f[2])+VERTTYPEMUL(w,rhs.f[3]);
		out.y = VERTTYPEMUL(x,rhs.f[4])+VERTTYPEMUL(y,rhs.f[5])+VERTTYPEMUL(z,rhs.f[6])+VERTTYPEMUL(w,rhs.f[7]);
		out.z = VERTTYPEMUL(x,rhs.f[8])+VERTTYPEMUL(y,rhs.f[9])+VERTTYPEMUL(z,rhs.f[10])+VERTTYPEMUL(w,rhs.f[11]);
		out.w = VERTTYPEMUL(x,rhs.f[12])+VERTTYPEMUL(y,rhs.f[13])+VERTTYPEMUL(z,rhs.f[14])+VERTTYPEMUL(w,rhs.f[15]);
		return out;
	}

/*!***************************************************************************
 @Function			*=
 @Input				rhs a Mat4
 @Returns			result of multiplication and assignment
 @Description		matrix multiplication and assignment operator for Vec4 and Mat4
****************************************************************************/
	Vec4& Vec4::operator*=(const Mat4& rhs)
	{
		VERTTYPE tx = VERTTYPEMUL(x,rhs.f[0])+VERTTYPEMUL(y,rhs.f[1])+VERTTYPEMUL(z,rhs.f[2])+VERTTYPEMUL(w,rhs.f[3]);
		VERTTYPE ty = VERTTYPEMUL(x,rhs.f[4])+VERTTYPEMUL(y,rhs.f[5])+VERTTYPEMUL(z,rhs.f[6])+VERTTYPEMUL(w,rhs.f[7]);
		VERTTYPE tz = VERTTYPEMUL(x,rhs.f[8])+VERTTYPEMUL(y,rhs.f[9])+VERTTYPEMUL(z,rhs.f[10])+VERTTYPEMUL(w,rhs.f[11]);
		w = VERTTYPEMUL(x,rhs.f[12])+VERTTYPEMUL(y,rhs.f[13])+VERTTYPEMUL(z,rhs.f[14])+VERTTYPEMUL(w,rhs.f[15]);
		x = tx;
		y = ty;
		z = tz;
		return *this;
	}

/*!***************************************************************************
** Mat3 3x3 matrix
****************************************************************************/
/*!***************************************************************************
 @Function			Mat3
 @Input				mat a Mat4
 @Description		constructor to form a Mat3 from a Mat4
****************************************************************************/
	Mat3::Mat3(const Mat4& mat)
	{
		VERTTYPE *dest = (VERTTYPE*)f, *src = (VERTTYPE*)mat.f;
		for(int i=0;i<3;i++)
		{
			for(int j=0;j<3;j++)
			{
				(*dest++) = (*src++);
			}
			src++;
		}
	}

/*!***************************************************************************
 @Function			RotationX
 @Input				angle the angle of rotation
 @Returns			rotation matrix
 @Description		generates a 3x3 rotation matrix about the X axis
****************************************************************************/
	Mat3 Mat3::RotationX(VERTTYPE angle)
	{
		Mat4 out;
		PVRTMatrixRotationX(out,angle);
		return Mat3(out);
	}
/*!***************************************************************************
 @Function			RotationY
 @Input				angle the angle of rotation
 @Returns			rotation matrix
 @Description		generates a 3x3 rotation matrix about the Y axis
****************************************************************************/
	Mat3 Mat3::RotationY(VERTTYPE angle)
	{
		Mat4 out;
		PVRTMatrixRotationY(out,angle);
		return Mat3(out);
	}
/*!***************************************************************************
 @Function			RotationZ
 @Input				angle the angle of rotation
 @Returns			rotation matrix
 @Description		generates a 3x3 rotation matrix about the Z axis
****************************************************************************/
	Mat3 Mat3::RotationZ(VERTTYPE angle)
	{
		Mat4 out;
		PVRTMatrixRotationZ(out,angle);
		return Mat3(out);
	}


/*!***************************************************************************
** Mat4 4x4 matrix
****************************************************************************/
/*!***************************************************************************
 @Function			RotationX
 @Input				angle the angle of rotation
 @Returns			rotation matrix
 @Description		generates a 4x4 rotation matrix about the X axis
****************************************************************************/
	Mat4 Mat4::RotationX(VERTTYPE angle)
	{
		Mat4 out;
		PVRTMatrixRotationX(out,angle);
		return out;
	}
/*!***************************************************************************
 @Function			RotationY
 @Input				angle the angle of rotation
 @Returns			rotation matrix
 @Description		generates a 4x4 rotation matrix about the Y axis
****************************************************************************/
	Mat4 Mat4::RotationY(VERTTYPE angle)
	{
		Mat4 out;
		PVRTMatrixRotationY(out,angle);
		return out;
	}
/*!***************************************************************************
 @Function			RotationZ
 @Input				angle the angle of rotation
 @Returns			rotation matrix
 @Description		generates a 4x4 rotation matrix about the Z axis
****************************************************************************/
	Mat4 Mat4::RotationZ(VERTTYPE angle)
	{
		Mat4 out;
		PVRTMatrixRotationZ(out,angle);
		return out;
	}

/*!***************************************************************************
 @Function			*
 @Input				rhs another Mat4
 @Returns			result of multiplication
 @Description		Matrix multiplication of two 4x4 matrices.
*****************************************************************************/
	Mat4 Mat4::operator*(const Mat4& rhs) const
	{
		Mat4 out;
		// col 1
		out.f[0] =	VERTTYPEMUL(f[0],rhs.f[0])+VERTTYPEMUL(f[4],rhs.f[1])+VERTTYPEMUL(f[8],rhs.f[2])+VERTTYPEMUL(f[12],rhs.f[3]);
		out.f[1] =	VERTTYPEMUL(f[1],rhs.f[0])+VERTTYPEMUL(f[5],rhs.f[1])+VERTTYPEMUL(f[9],rhs.f[2])+VERTTYPEMUL(f[13],rhs.f[3]);
		out.f[2] =	VERTTYPEMUL(f[2],rhs.f[0])+VERTTYPEMUL(f[6],rhs.f[1])+VERTTYPEMUL(f[10],rhs.f[2])+VERTTYPEMUL(f[14],rhs.f[3]);
		out.f[3] =	VERTTYPEMUL(f[3],rhs.f[0])+VERTTYPEMUL(f[7],rhs.f[1])+VERTTYPEMUL(f[11],rhs.f[2])+VERTTYPEMUL(f[15],rhs.f[3]);

		// col 2
		out.f[4] =	VERTTYPEMUL(f[0],rhs.f[4])+VERTTYPEMUL(f[4],rhs.f[5])+VERTTYPEMUL(f[8],rhs.f[6])+VERTTYPEMUL(f[12],rhs.f[7]);
		out.f[5] =	VERTTYPEMUL(f[1],rhs.f[4])+VERTTYPEMUL(f[5],rhs.f[5])+VERTTYPEMUL(f[9],rhs.f[6])+VERTTYPEMUL(f[13],rhs.f[7]);
		out.f[6] =	VERTTYPEMUL(f[2],rhs.f[4])+VERTTYPEMUL(f[6],rhs.f[5])+VERTTYPEMUL(f[10],rhs.f[6])+VERTTYPEMUL(f[14],rhs.f[7]);
		out.f[7] =	VERTTYPEMUL(f[3],rhs.f[4])+VERTTYPEMUL(f[7],rhs.f[5])+VERTTYPEMUL(f[11],rhs.f[6])+VERTTYPEMUL(f[15],rhs.f[7]);

		// col3
		out.f[8] =	VERTTYPEMUL(f[0],rhs.f[8])+VERTTYPEMUL(f[4],rhs.f[9])+VERTTYPEMUL(f[8],rhs.f[10])+VERTTYPEMUL(f[12],rhs.f[11]);
		out.f[9] =	VERTTYPEMUL(f[1],rhs.f[8])+VERTTYPEMUL(f[5],rhs.f[9])+VERTTYPEMUL(f[9],rhs.f[10])+VERTTYPEMUL(f[13],rhs.f[11]);
		out.f[10] =	VERTTYPEMUL(f[2],rhs.f[8])+VERTTYPEMUL(f[6],rhs.f[9])+VERTTYPEMUL(f[10],rhs.f[10])+VERTTYPEMUL(f[14],rhs.f[11]);
		out.f[11] =	VERTTYPEMUL(f[3],rhs.f[8])+VERTTYPEMUL(f[7],rhs.f[9])+VERTTYPEMUL(f[11],rhs.f[10])+VERTTYPEMUL(f[15],rhs.f[11]);

		// col3
		out.f[12] =	VERTTYPEMUL(f[0],rhs.f[12])+VERTTYPEMUL(f[4],rhs.f[13])+VERTTYPEMUL(f[8],rhs.f[14])+VERTTYPEMUL(f[12],rhs.f[15]);
		out.f[13] =	VERTTYPEMUL(f[1],rhs.f[12])+VERTTYPEMUL(f[5],rhs.f[13])+VERTTYPEMUL(f[9],rhs.f[14])+VERTTYPEMUL(f[13],rhs.f[15]);
		out.f[14] =	VERTTYPEMUL(f[2],rhs.f[12])+VERTTYPEMUL(f[6],rhs.f[13])+VERTTYPEMUL(f[10],rhs.f[14])+VERTTYPEMUL(f[14],rhs.f[15]);
		out.f[15] =	VERTTYPEMUL(f[3],rhs.f[12])+VERTTYPEMUL(f[7],rhs.f[13])+VERTTYPEMUL(f[11],rhs.f[14])+VERTTYPEMUL(f[15],rhs.f[15]);
		return out;
	}

}

/*****************************************************************************
End of file (PVRTVector.cpp)
*****************************************************************************/
