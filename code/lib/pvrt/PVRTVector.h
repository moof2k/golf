/******************************************************************************

 @File         PVRTVector.h

 @Title        

 @Copyright    Copyright (C) 2007 - 2008 by Imagination Technologies Limited.

 @Platform     ANSI compatible

 @Description  Vector and matrix mathematics library

******************************************************************************/
#ifndef __PVRTVECTOR_H__
#define __PVRTVECTOR_H__

#include "assert.h"
#include "PVRTGlobal.h"
#include "PVRTFixedPoint.h"
#include "PVRTMatrix.h"

#include <math.h>

namespace PVRT
{
/*!***************************************************************************
** Forward Declarations for vector and matrix structs
****************************************************************************/
	struct Vec4;
	struct Mat3;
	struct Mat4;

/*!***************************************************************************
** Vec3 3 component vector
****************************************************************************/
	struct Vec3 : public PVRTVECTOR3
	{
/*!***************************************************************************
** Constructors
****************************************************************************/
/*!***************************************************************************
 @Function			Vec3
 @Description		Blank constructor.
*****************************************************************************/
		Vec3(){}
/*!***************************************************************************
 @Function			Vec3
 @Input				fX	X component of vector
 @Input				fY	Y component of vector
 @Input				fZ	Z component of vector
 @Description		Simple constructor from 3 values.
*****************************************************************************/
		Vec3(VERTTYPE fX, VERTTYPE fY, VERTTYPE fZ)
		{
			x = fX;	y = fY;	z = fZ;
		}
/*!***************************************************************************
 @Function			Vec3
 @Input				fValue	a component value
 @Description		Constructor from a single value.
*****************************************************************************/
		Vec3(const VERTTYPE fValue)
		{
			x = fValue; y = fValue; z = fValue;
		}
/*!***************************************************************************
 @Function			Vec3
 @Input				pVec	an array
 @Description		Constructor from an array
*****************************************************************************/
		Vec3(const VERTTYPE* pVec)
		{
			x = (*pVec++); y = (*pVec++); z = *pVec;
		}
/*!***************************************************************************
 @Function			Vec3
 @Input				v4Vec a Vec4
 @Description		Constructor from a Vec4
*****************************************************************************/
		Vec3(const Vec4& v4Vec);
/*!***************************************************************************
** Operators
****************************************************************************/
/*!***************************************************************************
 @Function			+
 @Input				rhs another Vec3
 @Returns			result of addition
 @Description		componentwise addition operator for two Vec3s
*****************************************************************************/
		Vec3 operator+(const Vec3& rhs) const
		{
			Vec3 out;
			out.x = x+rhs.x;
			out.y = y+rhs.y;
			out.z = z+rhs.z;
			return out;
		}
/*!***************************************************************************
 @Function			-
 @Input				rhs another Vec3
 @Returns			result of subtraction
 @Description		componentwise subtraction operator for two Vec3s
****************************************************************************/
		Vec3 operator-(const Vec3& rhs) const
		{
			Vec3 out;
			out.x = x-rhs.x;
			out.y = y-rhs.y;
			out.z = z-rhs.z;
			return out;
		}

/*!***************************************************************************
 @Function			+=
 @Input				rhs another Vec3
 @Returns			result of addition
 @Description		componentwise addition and assignement operator for two Vec3s
****************************************************************************/
		Vec3& operator+=(const Vec3& rhs)
		{
			x +=rhs.x;
			y +=rhs.y;
			z +=rhs.z;
			return *this;
		}

/*!***************************************************************************
 @Function			-=
 @Input				rhs another Vec3
 @Returns			result of subtraction
 @Description		componentwise subtraction and assignement operator for two Vec3s
****************************************************************************/
		Vec3& operator-=(const Vec3& rhs)
		{
			x -=rhs.x;
			y -=rhs.y;
			z -=rhs.z;
			return *this;
		}

/*!***************************************************************************
 @Function			-
 @Input				rhs another Vec3
 @Returns			result of negation
 @Description		negation operator for a Vec3
****************************************************************************/
		friend Vec3 operator - (const Vec3& rhs) { return Vec3(rhs) *= -1; }

/*!***************************************************************************
 @Function			*
 @Input				lhs single value
 @Input				rhs a Vec3
 @Returns			result of negation
 @Description		negation operator for a Vec3
****************************************************************************/
		friend Vec3 operator*(const VERTTYPE lhs, const Vec3&  rhs)
		{
			Vec3 out;
			out.x = VERTTYPEDIV(lhs,rhs.x);
			out.y = VERTTYPEDIV(lhs,rhs.y);
			out.z = VERTTYPEDIV(lhs,rhs.z);
			return out;
		}

/*!***************************************************************************
 @Function			*
 @Input				lhs single value
 @Input				rhs a Vec3
 @Returns			result of negation
 @Description		negation operator for a Vec3
****************************************************************************/
		friend Vec3 operator/(const VERTTYPE lhs, const Vec3&  rhs)
		{
			Vec3 out;
			out.x = VERTTYPEDIV(lhs,rhs.x);
			out.y = VERTTYPEDIV(lhs,rhs.y);
			out.z = VERTTYPEDIV(lhs,rhs.z);
			return out;
		}

/*!***************************************************************************
 @Function			*
 @Input				rhs a Mat3
 @Returns			result of multiplication
 @Description		matrix multiplication operator Vec3 and Mat3
****************************************************************************/
		Vec3 operator*(const Mat3& rhs) const;

/*!***************************************************************************
 @Function			*=
 @Input				rhs a Mat3
 @Returns			result of multiplication and assignment
 @Description		matrix multiplication and assignment operator for Vec3 and Mat3
****************************************************************************/
		Vec3& operator*=(const Mat3& rhs);

/*!***************************************************************************
 @Function			*
 @Input				rhs a single value
 @Returns			result of multiplication
 @Description		componentwise multiplication by single dimension value for Vec3
****************************************************************************/
		Vec3 operator*(const VERTTYPE& rhs) const
		{
			Vec3 out;
			out.x = VERTTYPEMUL(x,rhs);
			out.y = VERTTYPEMUL(y,rhs);
			out.z = VERTTYPEMUL(z,rhs);
			return out;
		}

/*!***************************************************************************
 @Function			*
 @Input				rhs a single value
 @Returns			result of multiplication and assignment
 @Description		componentwise multiplication and assignement by single
					dimension value	for Vec3
****************************************************************************/
		Vec3& operator*=(const VERTTYPE& rhs)
		{
			x = VERTTYPEMUL(x,rhs);
			y = VERTTYPEMUL(y,rhs);
			z = VERTTYPEMUL(z,rhs);
			return *this;
		}

/*!***************************************************************************
 @Function			/
 @Input				rhs a single value
 @Returns			result of division
 @Description		componentwise division by single
					dimension value	for Vec3
****************************************************************************/
		Vec3 operator/(const VERTTYPE& rhs) const
		{
			Vec3 out;
			out.x = VERTTYPEDIV(x,rhs);
			out.y = VERTTYPEDIV(y,rhs);
			out.z = VERTTYPEDIV(z,rhs);
			return out;
		}

/*!***************************************************************************
 @Function			/=
 @Input				rhs a single value
 @Returns			result of division and assignment
 @Description		componentwise division and assignement by single
					dimension value	for Vec3
****************************************************************************/
		Vec3& operator/=(const VERTTYPE& rhs)
		{
			x = VERTTYPEDIV(x,rhs);
			y = VERTTYPEDIV(y,rhs);
			z = VERTTYPEDIV(z,rhs);
			return *this;
		}

		// FUNCTIONS
/*!***************************************************************************
 @Function			lenSqr
 @Returns			the square of the magnitude of the vector
 @Description		calculates the square of the magnitude of the vector
****************************************************************************/
		VERTTYPE lenSqr() const
		{
			return VERTTYPEMUL(x,x)+VERTTYPEMUL(y,y)+VERTTYPEMUL(z,z);
		}

/*!***************************************************************************
 @Function			length
 @Returns			the of the magnitude of the vector
 @Description		calculates the magnitude of the vector
****************************************************************************/
		VERTTYPE length() const
		{
			return (VERTTYPE) f2vt(sqrt(vt2f(lenSqr())));
		}

/*!***************************************************************************
 @Function			normalize
 @Returns			the normalized value of the vector
 @Description		normalizes the vector
****************************************************************************/
		Vec3 normalize()
		{
			VERTTYPE len = length();
			x =VERTTYPEDIV(x,len);
			y =VERTTYPEDIV(y,len);
			z =VERTTYPEDIV(z,len);
			return *this;
		}

/*!***************************************************************************
 @Function			normalized
 @Returns			returns the normalized value of the vector
 @Description		returns a normalized vector of the same direction as this
					vector
****************************************************************************/
		Vec3 normalized() const
		{
			Vec3 out;
			VERTTYPE len = length();
			out.x =VERTTYPEDIV(x,len);
			out.y =VERTTYPEDIV(y,len);
			out.z =VERTTYPEDIV(z,len);
			return out;
		}

/*!***************************************************************************
 @Function			dot
 @Input				rhs a single value
 @Returns			scalar product
 @Description		calculate the scalar product of two Vec3s
****************************************************************************/
		VERTTYPE dot(const Vec3& rhs)
		{
			return VERTTYPEMUL(x,rhs.x)+VERTTYPEMUL(y,rhs.y)+VERTTYPEMUL(z,rhs.z);
		}

/*!***************************************************************************
 @Function			dot
 @Returns			scalar product
 @Description		calculate the scalar product of two Vec3s
****************************************************************************/
		Vec3 cross(const Vec3& rhs)
		{
			Vec3 out;
			out.x = VERTTYPEMUL(y,rhs.z)-VERTTYPEMUL(z,rhs.y);
			out.y = VERTTYPEMUL(z,rhs.x)-VERTTYPEMUL(x,rhs.z);
			out.z = VERTTYPEMUL(x,rhs.y)-VERTTYPEMUL(y,rhs.x);
			return out;
		}

/*!***************************************************************************
 @Function			ptr
 @Returns			pointer
 @Description		returns a pointer to memory containing the values of the
					Vec3
****************************************************************************/
		VERTTYPE *ptr() { return (VERTTYPE*)this; }
	};

/*!***************************************************************************
** Vec4 4 component vector
****************************************************************************/
	struct Vec4 : public PVRTVECTOR4
	{
/*!***************************************************************************
** Constructors
****************************************************************************/
/*!***************************************************************************
 @Function			Vec4
 @Description		Blank constructor.
*****************************************************************************/
		Vec4(){}

/*!***************************************************************************
 @Function			Vec3
 @Description		Blank constructor.
*****************************************************************************/
		Vec4(const VERTTYPE vec)
		{
			x = vec; y = vec; z = vec; w = vec;
		}

/*!***************************************************************************
 @Function			multiple value constructor
 @Input				fX value of x component
 @Input				fY value of y component
 @Input				fZ value of z component
 @Input				fW value of w component
 @Description		Constructs a Vec4 from 4 separate values
****************************************************************************/
		Vec4(VERTTYPE fX, VERTTYPE fY, VERTTYPE fZ, VERTTYPE fW)
		{
			x = fX;	y = fY;	z = fZ;	w = fW;
		}

/*!***************************************************************************
 @Function			constructor from Vec3
 @Input				vec3 a Vec3
 @Input				fW value of w component
 @Description		Constructs a Vec4 from a vec3 and a w component
****************************************************************************/
		Vec4(const Vec3& vec3, VERTTYPE fW)
		{
			x = vec3.x;	y = vec3.y;	z = vec3.z;	w = fW;
		}

/*!***************************************************************************
 @Function			constructor from Vec3
 @Input				fX value of x component
 @Input				vec3 a Vec3
 @Description		Constructs a vec4 from a vec3 and a w component
****************************************************************************/
		Vec4(VERTTYPE fX, const Vec3& vec3)
		{
			x = fX;	y = vec3.x;	z = vec3.y;	w = vec3.z;
		}

/*!***************************************************************************
 @Function			constructor from array
 @Input				pVec a pointer to an array of four values
 @Description		Constructs a Vec4 from a pointer to an array of four values
****************************************************************************/
		Vec4(const VERTTYPE* pVec)
		{
			x = (*pVec++); y = (*pVec++); z= (*pVec++); w = *pVec;
		}

/*!***************************************************************************
** Vec4 Operators
****************************************************************************/
/*!***************************************************************************
 @Function			+
 @Input				rhs another Vec4
 @Returns			result of addition
 @Description		Addition operator for Vec4
****************************************************************************/
		Vec4 operator+(const Vec4& rhs) const
		{
			Vec4 out;
			out.x = x+rhs.x;
			out.y = y+rhs.y;
			out.z = z+rhs.z;
			out.w = w+rhs.w;
			return out;
		}

/*!***************************************************************************
 @Function			-
 @Input				rhs another Vec4
 @Returns			result of subtraction
 @Description		Subtraction operator for Vec4
****************************************************************************/
		Vec4 operator-(const Vec4& rhs) const
		{
			Vec4 out;
			out.x = x-rhs.x;
			out.y = y-rhs.y;
			out.z = z-rhs.z;
			out.w = w-rhs.w;
			return out;
		}

/*!***************************************************************************
 @Function			+=
 @Input				rhs another Vec4
 @Returns			result of addition and assignment
 @Description		Addition and assignment operator for Vec4
****************************************************************************/
		Vec4& operator+=(const Vec4& rhs)
		{
			x +=rhs.x;
			y +=rhs.y;
			z +=rhs.z;
			w +=rhs.w;
			return *this;
		}

/*!***************************************************************************
 @Function			-=
 @Input				rhs another Vec4
 @Returns			result of subtraction and assignment
 @Description		Subtraction and assignment operator for Vec4
****************************************************************************/
		Vec4& operator-=(const Vec4& rhs)
		{
			x -=rhs.x;
			y -=rhs.y;
			z -=rhs.z;
			w -=rhs.w;
			return *this;
		}

/*!***************************************************************************
 @Function			*
 @Input				rhs a Mat4
 @Returns			result of multiplication
 @Description		matrix multiplication for Vec4 and Mat4
****************************************************************************/
		Vec4 operator*(const Mat4& rhs) const;

/*!***************************************************************************
 @Function			*=
 @Input				rhs a Mat4
 @Returns			result of multiplication and assignement
 @Description		matrix multiplication and assignment for Vec4 and Mat4
****************************************************************************/
		Vec4& operator*=(const Mat4& rhs);

/*!***************************************************************************
 @Function			*
 @Input				rhs a single dimension value
 @Returns			result of multiplication
 @Description		componentwise multiplication of a Vec4 by a single value
****************************************************************************/
		Vec4 operator*(const VERTTYPE& rhs) const
		{
			Vec4 out;
			out.x = VERTTYPEMUL(x,rhs);
			out.y = VERTTYPEMUL(y,rhs);
			out.z = VERTTYPEMUL(z,rhs);
			out.w = VERTTYPEMUL(w,rhs);
			return out;
		}

/*!***************************************************************************
 @Function			*=
 @Input				rhs a single dimension value
 @Returns			result of multiplication and assignment
 @Description		componentwise multiplication and assignment of a Vec4 by
					a single value
****************************************************************************/
		Vec4& operator*=(const VERTTYPE& rhs)
		{
			x = VERTTYPEMUL(x,rhs);
			y = VERTTYPEMUL(y,rhs);
			z = VERTTYPEMUL(z,rhs);
			w = VERTTYPEMUL(w,rhs);
			return *this;
		}

/*!***************************************************************************
 @Function			/
 @Input				rhs a single dimension value
 @Returns			result of division
 @Description		componentwise division of a Vec4 by a single value
****************************************************************************/
		Vec4 operator/(const VERTTYPE& rhs) const
		{
			Vec4 out;
			out.x = VERTTYPEDIV(x,rhs);
			out.y = VERTTYPEDIV(y,rhs);
			out.z = VERTTYPEDIV(z,rhs);
			out.w = VERTTYPEDIV(w,rhs);
			return out;
		}

/*!***************************************************************************
 @Function			/=
 @Input				rhs a single dimension value
 @Returns			result of division and assignment
 @Description		componentwise division and assignment of a Vec4 by
					a single value
****************************************************************************/
		Vec4& operator/=(const VERTTYPE& rhs)
		{
			x = VERTTYPEDIV(x,rhs);
			y = VERTTYPEDIV(y,rhs);
			z = VERTTYPEDIV(z,rhs);
			w = VERTTYPEDIV(w,rhs);
			return *this;
		}

/*!***************************************************************************
 @Function			*
 @Input				lhs a single dimension value
 @Input				rhs a Vec4
 @Returns			result of muliplication
 @Description		componentwise multiplication of a Vec4 by
					a single value
****************************************************************************/
	friend Vec4 operator*(const VERTTYPE lhs, const Vec4&  rhs)
	{
		Vec4 out;
		out.x = VERTTYPEDIV(lhs,rhs.x);
		out.y = VERTTYPEDIV(lhs,rhs.y);
		out.z = VERTTYPEDIV(lhs,rhs.z);
		out.w = VERTTYPEDIV(lhs,rhs.w);
		return out;
	}

/*!***************************************************************************
** Functions
****************************************************************************/
/*!***************************************************************************
 @Function			lenSqr
 @Returns			square of the magnitude of the vector
 @Description		calculates the square of the magnitude of the vector
****************************************************************************/
		VERTTYPE lenSqr() const
		{
			return VERTTYPEMUL(x,x)+VERTTYPEMUL(y,y)+VERTTYPEMUL(z,z)+VERTTYPEMUL(w,w);
		}

/*!***************************************************************************
 @Function			length
 @Returns			the magnitude of the vector
 @Description		calculates the magnitude of the vector
****************************************************************************/
		VERTTYPE length() const
		{
			return (VERTTYPE) f2vt(sqrt(vt2f(lenSqr())));
		}

/*!***************************************************************************
 @Function			normalize
 @Returns			normalized vector
 @Description		calculates the normalized value of a Vec4
****************************************************************************/
		Vec4 normalize()
		{
			VERTTYPE len = length();
			x =x/len;
			y =y/len;
			z =z/len;
			w =w/len;
			return *this;
		}
/*!***************************************************************************
 @Function			normalized
 @Returns			normalized vector
 @Description		returns a normalized vector of the same direction as this
					vector
****************************************************************************/
		Vec4 normalized() const
		{
			Vec4 out;
			VERTTYPE len = length();
			out.x =x/len;
			out.y =y/len;
			out.z =z/len;
			out.w =w/len;
			return out;
		}

/*!***************************************************************************
 @Function			dot
 @Returns			scalar product
 @Description		returns a normalized vector of the same direction as this
					vector
****************************************************************************/
		VERTTYPE dot(const Vec4& rhs)
		{
			return VERTTYPEMUL(x,rhs.x)+VERTTYPEMUL(y,rhs.y)+VERTTYPEMUL(z,rhs.z)+VERTTYPEMUL(w,rhs.w);
		}

/*!***************************************************************************
 @Function			ptr
 @Returns			pointer to vector values
 @Description		returns a pointer to memory containing the values of the
					Vec3
****************************************************************************/
		VERTTYPE *ptr() { return (VERTTYPE*)this; }
	};

/*!***************************************************************************
** Mat3 3x3 Matrix
****************************************************************************/
	struct Mat3 : public PVRTMATRIX3
	{
/*!***************************************************************************
** Constructors
****************************************************************************/
/*!***************************************************************************
 @Function			Mat3
 @Description		Blank constructor.
*****************************************************************************/
		Mat3(){}
/*!***************************************************************************
 @Function			Mat3
 @Input				pMat an array of values for the matrix
 @Description		Constructor from array.
*****************************************************************************/
		Mat3(const VERTTYPE* pMat)
		{
			VERTTYPE* ptr = f;
			for(int i=0;i<9;i++)
			{
				(*ptr++)=(*pMat++);
			}
		}

/*!***************************************************************************
 @Function			Mat3
 @Input				m0 matrix value
 @Input				m1 matrix value
 @Input				m2 matrix value
 @Input				m3 matrix value
 @Input				m4 matrix value
 @Input				m5 matrix value
 @Input				m6 matrix value
 @Input				m7 matrix value
 @Input				m8 matrix value
 @Description		Constructor from distinct values.
*****************************************************************************/
		Mat3(VERTTYPE m0,VERTTYPE m1,VERTTYPE m2,
			VERTTYPE m3,VERTTYPE m4,VERTTYPE m5,
			VERTTYPE m6,VERTTYPE m7,VERTTYPE m8)
		{
			f[0]=m0;f[1]=m1;f[2]=m2;
			f[3]=m3;f[4]=m4;f[5]=m5;
			f[6]=m6;f[7]=m7;f[8]=m8;
		}

/*!***************************************************************************
 @Function			Mat3
 @Input				mat - a Mat4
 @Description		Constructor from 4x4 matrix - uses top left values
*****************************************************************************/
		Mat3(const Mat4& mat);

/****************************************************************************
** Mat3 OPERATORS
****************************************************************************/
/*!***************************************************************************
 @Function			()
 @Input				r - row of matrix
 @Input				c - column of matrix
 @Returns			value of element
 @Description		Returns value of the element at row r and colun c of the
					Mat3
*****************************************************************************/
		VERTTYPE& operator()(const int r, const int c)
		{
			return f[c*3+r];
		}
/*!***************************************************************************
 @Function			()
 @Input				r - row of matrix
 @Input				c - column of matrix
 @Returns			value of element
 @Description		Returns value of the element at row r and colun c of the
					Mat3
*****************************************************************************/
		const VERTTYPE& operator()(const int r, const int c) const
		{
			return f[c*3+r];
		}

/*!***************************************************************************
 @Function			*
 @Input				rhs another Mat3
 @Returns			result of multiplication
 @Description		Matrix multiplication of two 3x3 matrices.
*****************************************************************************/
		Mat3 operator*(const Mat3& rhs) const
		{
			Mat3 out;
			// col 1
			out.f[0] =	VERTTYPEMUL(f[0],rhs.f[0])+VERTTYPEMUL(f[3],rhs.f[1])+VERTTYPEMUL(f[6],rhs.f[2]);
			out.f[1] =	VERTTYPEMUL(f[1],rhs.f[0])+VERTTYPEMUL(f[4],rhs.f[1])+VERTTYPEMUL(f[7],rhs.f[2]);
			out.f[2] =	VERTTYPEMUL(f[2],rhs.f[0])+VERTTYPEMUL(f[5],rhs.f[1])+VERTTYPEMUL(f[8],rhs.f[2]);

			// col 2
			out.f[3] =	VERTTYPEMUL(f[0],rhs.f[3])+VERTTYPEMUL(f[3],rhs.f[4])+VERTTYPEMUL(f[6],rhs.f[5]);
			out.f[4] =	VERTTYPEMUL(f[1],rhs.f[3])+VERTTYPEMUL(f[4],rhs.f[4])+VERTTYPEMUL(f[7],rhs.f[5]);
			out.f[5] =	VERTTYPEMUL(f[2],rhs.f[3])+VERTTYPEMUL(f[5],rhs.f[4])+VERTTYPEMUL(f[8],rhs.f[5]);

			// col3
			out.f[6] =	VERTTYPEMUL(f[0],rhs.f[6])+VERTTYPEMUL(f[3],rhs.f[7])+VERTTYPEMUL(f[6],rhs.f[8]);
			out.f[7] =	VERTTYPEMUL(f[1],rhs.f[6])+VERTTYPEMUL(f[4],rhs.f[7])+VERTTYPEMUL(f[7],rhs.f[8]);
			out.f[8] =	VERTTYPEMUL(f[2],rhs.f[6])+VERTTYPEMUL(f[5],rhs.f[7])+VERTTYPEMUL(f[8],rhs.f[8]);
			return out;
		}

/*!***************************************************************************
 @Function			+
 @Input				rhs another Mat3
 @Returns			result of addition
 @Description		element by element addition operator.
*****************************************************************************/
		Mat3 operator+(const Mat3& rhs) const
		{
			Mat3 out;
			VERTTYPE const *lptr = f, *rptr = rhs.f;
			VERTTYPE *outptr = out.f;
			for(int i=0;i<9;i++)
			{
				(*outptr++) = (*lptr++) + (*rptr++);
			}
			return out;
		}

/*!***************************************************************************
 @Function			-
 @Input				rhs another Mat3
 @Returns			result of subtraction
 @Description		element by element subtraction operator.
*****************************************************************************/
		Mat3 operator-(const Mat3& rhs) const
		{
			Mat3 out;
			VERTTYPE const *lptr = f, *rptr = rhs.f;
			VERTTYPE *outptr = out.f;
			for(int i=0;i<9;i++)
			{
				(*outptr++) = (*lptr++) - (*rptr++);
			}
			return out;
		}

/*!***************************************************************************
 @Function			+=
 @Input				rhs another Mat3
 @Returns			result of addition and assignment
 @Description		element by element addition and assignment operator.
*****************************************************************************/
		Mat3& operator+=(const Mat3& rhs)
		{
			VERTTYPE *lptr = f;
			VERTTYPE const *rptr = rhs.f;
			for(int i=0;i<9;i++)
			{
				(*lptr++) += (*rptr++);
			}
			return *this;
		}

/*!***************************************************************************
 @Function			-=
 @Input				rhs another Mat3
 @Returns			result of subtraction and assignment
 @Description		element by element subtraction and assignment operator.
*****************************************************************************/
		Mat3& operator-=(const Mat3& rhs)
		{
			VERTTYPE *lptr = f;
			VERTTYPE const *rptr = rhs.f;
			for(int i=0;i<9;i++)
			{
				(*lptr++) -= (*rptr++);
			}
			return *this;
		}

/*!***************************************************************************
 @Function			*=
 @Input				rhs another Mat3
 @Returns			result of multiplication and assignment
 @Description		Matrix multiplication and assignment of two 3x3 matrices.
*****************************************************************************/
		Mat3& operator*=(const Mat3& rhs)
		{
			Mat3 out;
			// col 1
			out.f[0] =	VERTTYPEMUL(f[0],rhs.f[0])+VERTTYPEMUL(f[3],rhs.f[1])+VERTTYPEMUL(f[6],rhs.f[2]);
			out.f[1] =	VERTTYPEMUL(f[1],rhs.f[0])+VERTTYPEMUL(f[4],rhs.f[1])+VERTTYPEMUL(f[7],rhs.f[2]);
			out.f[2] =	VERTTYPEMUL(f[2],rhs.f[0])+VERTTYPEMUL(f[5],rhs.f[1])+VERTTYPEMUL(f[8],rhs.f[2]);

			// col 2
			out.f[3] =	VERTTYPEMUL(f[0],rhs.f[3])+VERTTYPEMUL(f[3],rhs.f[4])+VERTTYPEMUL(f[6],rhs.f[5]);
			out.f[4] =	VERTTYPEMUL(f[1],rhs.f[3])+VERTTYPEMUL(f[4],rhs.f[4])+VERTTYPEMUL(f[7],rhs.f[5]);
			out.f[5] =	VERTTYPEMUL(f[2],rhs.f[3])+VERTTYPEMUL(f[5],rhs.f[4])+VERTTYPEMUL(f[8],rhs.f[5]);

			// col3
			out.f[6] =	VERTTYPEMUL(f[0],rhs.f[6])+VERTTYPEMUL(f[3],rhs.f[7])+VERTTYPEMUL(f[6],rhs.f[8]);
			out.f[7] =	VERTTYPEMUL(f[1],rhs.f[6])+VERTTYPEMUL(f[4],rhs.f[7])+VERTTYPEMUL(f[7],rhs.f[8]);
			out.f[8] =	VERTTYPEMUL(f[2],rhs.f[6])+VERTTYPEMUL(f[5],rhs.f[7])+VERTTYPEMUL(f[8],rhs.f[8]);
			*this = out;
			return *this;
		}

/*!***************************************************************************
 @Function			*
 @Input				rhs a single value
 @Returns			result of multiplication and assignment
 @Description		element multiplication by a single value.
*****************************************************************************/
		Mat3& operator*(const VERTTYPE rhs)
		{
			for (int i=0; i<9; ++i)
			{
				f[i]*=rhs;
			}
			return *this;
		}
/*!***************************************************************************
 @Function			*=
 @Input				rhs a single value
 @Returns			result of multiplication and assignment
 @Description		element multiplication and assignment by a single value.
*****************************************************************************/
		Mat3& operator*=(const VERTTYPE rhs)
		{
			for (int i=0; i<9; ++i)
			{
				f[i]*=rhs;
			}
			return *this;
		}

/*!***************************************************************************
 @Function			*
 @Input				rhs another Vec3
 @Returns			result of multiplication
 @Description		Matrix multiplication of 3x3 matrix and vec3
*****************************************************************************/
		Vec3 operator*(const Vec3& rhs) const
		{
			Vec3 out;
			out.x = VERTTYPEMUL(rhs.x,f[0])+VERTTYPEMUL(rhs.y,f[3])+VERTTYPEMUL(rhs.z,f[6]);
			out.y = VERTTYPEMUL(rhs.x,f[1])+VERTTYPEMUL(rhs.y,f[4])+VERTTYPEMUL(rhs.z,f[7]);
			out.z = VERTTYPEMUL(rhs.x,f[2])+VERTTYPEMUL(rhs.y,f[5])+VERTTYPEMUL(rhs.z,f[8]);

			return out;
		}


		// FUNCTIONS
/*!***************************************************************************
** Functions
*****************************************************************************/
/*!***************************************************************************
 @Function			determinant
 @Returns			result of multiplication
 @Description		Matrix multiplication and assignment of 3x3 matrix and vec3
*****************************************************************************/
		VERTTYPE determinant() const
		{
			return VERTTYPEMUL(f[0],(VERTTYPEMUL(f[4],f[8])-VERTTYPEMUL(f[7],f[5])))
				- VERTTYPEMUL(f[3],(VERTTYPEMUL(f[1],f[8])-VERTTYPEMUL(f[7],f[2])))
				+ VERTTYPEMUL(f[6],(VERTTYPEMUL(f[1],f[5])-VERTTYPEMUL(f[4],f[2])));
		}

/*!***************************************************************************
 @Function			inverse
 @Returns			inverse mat3
 @Description		Calculates multiplicative inverse of this matrix
*****************************************************************************/
		Mat3 inverse() const
		{
			Mat3 out;


			VERTTYPE recDet = determinant();
			_ASSERT(recDet!=0);
			recDet = VERTTYPEDIV(f2vt(1.0f),recDet);

			//TODO: deal with singular matrices with more than just an assert

			// inverse is 1/det * adjoint of M

			// adjoint is transpose of cofactor matrix

			// do transpose and cofactors in one step

			out.f[0] = VERTTYPEMUL(f[4],f[8]) - VERTTYPEMUL(f[5],f[7]);
			out.f[1] = VERTTYPEMUL(f[2],f[7]) - VERTTYPEMUL(f[1],f[8]);
			out.f[2] = VERTTYPEMUL(f[1],f[5]) - VERTTYPEMUL(f[2],f[4]);

			out.f[3] = VERTTYPEMUL(f[5],f[6]) - VERTTYPEMUL(f[3],f[8]);
			out.f[4] = VERTTYPEMUL(f[0],f[8]) - VERTTYPEMUL(f[2],f[6]);
			out.f[5] = VERTTYPEMUL(f[2],f[3]) - VERTTYPEMUL(f[0],f[5]);

			out.f[6] = VERTTYPEMUL(f[3],f[7]) - VERTTYPEMUL(f[4],f[6]);
			out.f[7] = VERTTYPEMUL(f[1],f[6]) - VERTTYPEMUL(f[0],f[7]);
			out.f[8] = VERTTYPEMUL(f[0],f[4]) - VERTTYPEMUL(f[1],f[3]);

			out *= recDet;
			return out;
		}

/*!***************************************************************************
 @Function			transpose
 @Returns			transpose 3x3 matrix
 @Description		Calculates the transpose of this matrix
*****************************************************************************/
		Mat3 transpose() const
		{
			Mat3 out;
			out.f[0] = f[0];	out.f[3] = f[1];	out.f[6] = f[2];
			out.f[1] = f[3];	out.f[4] = f[4];	out.f[7] = f[5];
			out.f[2] = f[6];	out.f[5] = f[7];	out.f[8] = f[8];
			return out;
		}

/*!***************************************************************************
 @Function			ptr
 @Returns			pointer to an array of the elements of this Mat3
 @Description		Calculates transpose of this matrix
*****************************************************************************/
		VERTTYPE *ptr() { return (VERTTYPE*)&f; }

/*!***************************************************************************
** Static factory functions
*****************************************************************************/
/*!***************************************************************************
 @Function			Identity
 @Returns			a Mat3 representation of the 3x3 identity matrix
 @Description		Generates an identity matrix
*****************************************************************************/
		static Mat3 Identity()
		{
			Mat3 out;
			out.f[0] = 1;out.f[1] = 0;out.f[2] = 0;
			out.f[3] = 0;out.f[4] = 1;out.f[5] = 0;
			out.f[6] = 0;out.f[7] = 0;out.f[8] = 1;
			return out;
		}

/*!***************************************************************************
@Function			RotationX
@Returns			a Mat3 corresponding to the requested rotation
@Description		Calculates a matrix corresponding to a rotation of angle
					degrees about the X axis
*****************************************************************************/
		static Mat3 RotationX(VERTTYPE angle);

/*!***************************************************************************
 @Function			RotationY
 @Returns			a Mat3 corresponding to the requested rotation
 @Description		Calculates a matrix corresponding to a rotation of angle
					degrees about the Y axis
*****************************************************************************/
		static Mat3 RotationY(VERTTYPE angle);

/*!***************************************************************************
 @Function			RotationZ
 @Returns			a Mat3 corresponding to the requested rotation
 @Description		Calculates a matrix corresponding to a rotation of angle
					degrees about the Z axis
*****************************************************************************/
		static Mat3 RotationZ(VERTTYPE angle);

/*!***************************************************************************
 @Function			Scale
 @Returns			a Mat3 corresponding to the requested scaling transformation
 @Description		Calculates a matrix corresponding to scaling of fx, fy and fz
					times in each axis.
*****************************************************************************/
		static Mat3 Scale(const VERTTYPE fx,const VERTTYPE fy,const VERTTYPE fz)
		{
			return Mat3(fx,0,0,
				0,fy,0,
				0,0,fz);
		}


/*!***************************************************************************
 @Function			Translation
 @Returns			a Mat3 corresponding to the requested translation
 @Description		Calculates a matrix corresponding to a transformation
					of tx and ty times in each axis.
*****************************************************************************/
		static Mat3 Translation(const VERTTYPE tx, const VERTTYPE ty)
		{
			return Mat3( 1,    0,  0,
				0,    1,  0,
				tx,  ty,  1);
		}

	};

/*!***************************************************************************
** Mat4 4x4 Matrix
****************************************************************************/
	struct Mat4 : public PVRTMATRIX
	{
/*!***************************************************************************
** Constructors
****************************************************************************/
/*!***************************************************************************
 @Function			Mat4
 @Description		Blank constructor.
*****************************************************************************/
		Mat4(){}
/*!***************************************************************************
 @Function			Mat4
 @Input				m0 matrix value
 @Input				m1 matrix value
 @Input				m2 matrix value
 @Input				m3 matrix value
 @Input				m4 matrix value
 @Input				m5 matrix value
 @Input				m6 matrix value
 @Input				m7 matrix value
 @Input				m8 matrix value
 @Input				m9 matrix value
 @Input				m10 matrix value
 @Input				m11 matrix value
 @Input				m12 matrix value
 @Input				m13 matrix value
 @Input				m14 matrix value
 @Input				m15 matrix value
 @Description		Constructor from array.
*****************************************************************************/
		Mat4(VERTTYPE m0,VERTTYPE m1,VERTTYPE m2,VERTTYPE m3,
			VERTTYPE m4,VERTTYPE m5,VERTTYPE m6,VERTTYPE m7,
			VERTTYPE m8,VERTTYPE m9,VERTTYPE m10,VERTTYPE m11,
			VERTTYPE m12,VERTTYPE m13,VERTTYPE m14,VERTTYPE m15)
		{
			f[0]=m0;f[1]=m1;f[2]=m2;f[3]=m3;
			f[4]=m4;f[5]=m5;f[6]=m6;f[7]=m7;
			f[8]=m8;f[9]=m9;f[10]=m10;f[11]=m11;
			f[12]=m12;f[13]=m13;f[14]=m14;f[15]=m15;
		}
/*!***************************************************************************
 @Function			Mat4
 @Input				mat A pointer to an array of 16 VERTTYPEs
 @Description		Constructor from distinct values.
*****************************************************************************/
		Mat4(const VERTTYPE* mat)
		{
			VERTTYPE* ptr = f;
			for(int i=0;i<16;i++)
			{
				(*ptr++)=(*mat++);
			}
		}

/****************************************************************************
** Mat4 OPERATORS
****************************************************************************/
/*!***************************************************************************
 @Function			()
 @Input				r - row of matrix
 @Input				c - column of matrix
 @Returns			value of element
 @Description		Returns value of the element at row r and colun c of the
					Mat4
*****************************************************************************/
		VERTTYPE& operator()(const int r, const int c)
		{
			return f[c*4+r];
		}

/*!***************************************************************************
 @Function			()
 @Input				r - row of matrix
 @Input				c - column of matrix
 @Returns			value of element
 @Description		Returns value of the element at row r and colun c of the
					Mat4
*****************************************************************************/
		const VERTTYPE& operator()(const int r, const int c) const
		{
			return f[c*4+r];
		}

/*!***************************************************************************
 @Function			*
 @Input				rhs another Mat4
 @Returns			result of multiplication
 @Description		Matrix multiplication of two 4x4 matrices.
*****************************************************************************/
		Mat4 operator*(const Mat4& rhs) const;

/*!***************************************************************************
 @Function			+
 @Input				rhs another Mat4
 @Returns			result of addition
 @Description		element by element addition operator.
*****************************************************************************/
		Mat4 operator+(const Mat4& rhs) const
		{
			Mat4 out;
			VERTTYPE const *lptr = f, *rptr = rhs.f;
			VERTTYPE *outptr = out.f;
			for(int i=0;i<16;i++)
			{
				(*outptr++) = (*lptr++) + (*rptr++);
			}
			return out;
		}

/*!***************************************************************************
 @Function			-
 @Input				rhs another Mat4
 @Returns			result of subtraction
 @Description		element by element subtraction operator.
*****************************************************************************/
		Mat4 operator-(const Mat4& rhs) const
		{
			Mat4 out;
			for(int i=0;i<16;i++)
			{
				out.f[i] = f[i]-rhs.f[i];
			}
			return out;
		}

/*!***************************************************************************
 @Function			+=
 @Input				rhs another Mat4
 @Returns			result of addition and assignment
 @Description		element by element addition and assignment operator.
*****************************************************************************/
		Mat4& operator+=(const Mat4& rhs)
		{
			VERTTYPE *lptr = f;
			VERTTYPE const *rptr = rhs.f;
			for(int i=0;i<16;i++)
			{
				(*lptr++) += (*rptr++);
			}
			return *this;
		}

/*!***************************************************************************
 @Function			-=
 @Input				rhs another Mat4
 @Returns			result of subtraction and assignment
 @Description		element by element subtraction and assignment operator.
*****************************************************************************/
		Mat4& operator-=(const Mat4& rhs)
		{
			VERTTYPE *lptr = f;
			VERTTYPE const *rptr = rhs.f;
			for(int i=0;i<16;i++)
			{
				(*lptr++) -= (*rptr++);
			}
			return *this;
		}


/*!***************************************************************************
 @Function			*=
 @Input				rhs another Mat4
 @Returns			result of multiplication and assignment
 @Description		Matrix multiplication and assignment of two 4x4 matrices.
*****************************************************************************/
		Mat4& operator*=(const Mat4& rhs)
		{
			Mat4 result;
			// col 0
			result.f[0] =	VERTTYPEMUL(f[0],rhs.f[0])+VERTTYPEMUL(f[4],rhs.f[1])+VERTTYPEMUL(f[8],rhs.f[2])+VERTTYPEMUL(f[12],rhs.f[3]);
			result.f[1] =	VERTTYPEMUL(f[1],rhs.f[0])+VERTTYPEMUL(f[5],rhs.f[1])+VERTTYPEMUL(f[9],rhs.f[2])+VERTTYPEMUL(f[13],rhs.f[3]);
			result.f[2] =	VERTTYPEMUL(f[2],rhs.f[0])+VERTTYPEMUL(f[6],rhs.f[1])+VERTTYPEMUL(f[10],rhs.f[2])+VERTTYPEMUL(f[14],rhs.f[3]);
			result.f[3] =	VERTTYPEMUL(f[3],rhs.f[0])+VERTTYPEMUL(f[7],rhs.f[1])+VERTTYPEMUL(f[11],rhs.f[2])+VERTTYPEMUL(f[15],rhs.f[3]);

			// col 1
			result.f[4] =	VERTTYPEMUL(f[0],rhs.f[4])+VERTTYPEMUL(f[4],rhs.f[5])+VERTTYPEMUL(f[8],rhs.f[6])+VERTTYPEMUL(f[12],rhs.f[7]);
			result.f[5] =	VERTTYPEMUL(f[1],rhs.f[4])+VERTTYPEMUL(f[5],rhs.f[5])+VERTTYPEMUL(f[9],rhs.f[6])+VERTTYPEMUL(f[13],rhs.f[7]);
			result.f[6] =	VERTTYPEMUL(f[2],rhs.f[4])+VERTTYPEMUL(f[6],rhs.f[5])+VERTTYPEMUL(f[10],rhs.f[6])+VERTTYPEMUL(f[14],rhs.f[7]);
			result.f[7] =	VERTTYPEMUL(f[3],rhs.f[4])+VERTTYPEMUL(f[7],rhs.f[5])+VERTTYPEMUL(f[11],rhs.f[6])+VERTTYPEMUL(f[15],rhs.f[7]);

			// col 2
			result.f[8] =	VERTTYPEMUL(f[0],rhs.f[8])+VERTTYPEMUL(f[4],rhs.f[9])+VERTTYPEMUL(f[8],rhs.f[10])+VERTTYPEMUL(f[12],rhs.f[11]);
			result.f[9] =	VERTTYPEMUL(f[1],rhs.f[8])+VERTTYPEMUL(f[5],rhs.f[9])+VERTTYPEMUL(f[9],rhs.f[10])+VERTTYPEMUL(f[13],rhs.f[11]);
			result.f[10] =	VERTTYPEMUL(f[2],rhs.f[8])+VERTTYPEMUL(f[6],rhs.f[9])+VERTTYPEMUL(f[10],rhs.f[10])+VERTTYPEMUL(f[14],rhs.f[11]);
			result.f[11] =	VERTTYPEMUL(f[3],rhs.f[8])+VERTTYPEMUL(f[7],rhs.f[9])+VERTTYPEMUL(f[11],rhs.f[10])+VERTTYPEMUL(f[15],rhs.f[11]);

			// col 3
			result.f[12] =	VERTTYPEMUL(f[0],rhs.f[12])+VERTTYPEMUL(f[4],rhs.f[13])+VERTTYPEMUL(f[8],rhs.f[14])+VERTTYPEMUL(f[12],rhs.f[15]);
			result.f[13] =	VERTTYPEMUL(f[1],rhs.f[12])+VERTTYPEMUL(f[5],rhs.f[13])+VERTTYPEMUL(f[9],rhs.f[14])+VERTTYPEMUL(f[13],rhs.f[15]);
			result.f[14] =	VERTTYPEMUL(f[2],rhs.f[12])+VERTTYPEMUL(f[6],rhs.f[13])+VERTTYPEMUL(f[10],rhs.f[14])+VERTTYPEMUL(f[14],rhs.f[15]);
			result.f[15] =	VERTTYPEMUL(f[3],rhs.f[12])+VERTTYPEMUL(f[7],rhs.f[13])+VERTTYPEMUL(f[11],rhs.f[14])+VERTTYPEMUL(f[15],rhs.f[15]);

			*this = result;
			return *this;
		}

/*!***************************************************************************
 @Function			*
 @Input				rhs a single value
 @Returns			result of multiplication and assignment
 @Description		element multiplication by a single value.
*****************************************************************************/
		Mat4& operator*(const VERTTYPE rhs)
		{
			for (int i=0; i<16; ++i)
			{
				f[i]*=rhs;
			}
			return *this;
		}
/*!***************************************************************************
 @Function			*=
 @Input				rhs a single value
 @Returns			result of multiplication and assignment
 @Description		element multiplication and assignment by a single value.
*****************************************************************************/
		Mat4& operator*=(const VERTTYPE rhs)
		{
			for (int i=0; i<16; ++i)
			{
				f[i]*=rhs;
			}
			return *this;
		}

/*!***************************************************************************
 @Function			*
 @Input				rhs a Vec4
 @Returns			result of multiplication
 @Description		Matrix multiplication of 4x4 matrix and vec3
*****************************************************************************/
		Vec4 operator*(const Vec4& rhs) const
		{
			Vec4 out;
			out.x = VERTTYPEMUL(rhs.x,f[0])+VERTTYPEMUL(rhs.y,f[4])+VERTTYPEMUL(rhs.z,f[8])+VERTTYPEMUL(rhs.w,f[12]);
			out.y = VERTTYPEMUL(rhs.x,f[1])+VERTTYPEMUL(rhs.y,f[5])+VERTTYPEMUL(rhs.z,f[9])+VERTTYPEMUL(rhs.w,f[13]);
			out.z = VERTTYPEMUL(rhs.x,f[2])+VERTTYPEMUL(rhs.y,f[6])+VERTTYPEMUL(rhs.z,f[10])+VERTTYPEMUL(rhs.w,f[14]);
			out.w = VERTTYPEMUL(rhs.x,f[3])+VERTTYPEMUL(rhs.y,f[7])+VERTTYPEMUL(rhs.z,f[11])+VERTTYPEMUL(rhs.w,f[15]);

			return out;
		}

/*!***************************************************************************
 @Function			*=
 @Input				rhs a Vec4
 @Returns			result of multiplication and assignment
 @Description		Matrix multiplication and assignment of 4x4 matrix and vec3
*****************************************************************************/
		Vec4 operator*=(const Vec4& rhs) const
		{
			Vec4 out;
			out.x = VERTTYPEMUL(rhs.x,f[0])+VERTTYPEMUL(rhs.y,f[4])+VERTTYPEMUL(rhs.z,f[8])+VERTTYPEMUL(rhs.w,f[12]);
			out.y = VERTTYPEMUL(rhs.x,f[1])+VERTTYPEMUL(rhs.y,f[5])+VERTTYPEMUL(rhs.z,f[9])+VERTTYPEMUL(rhs.w,f[13]);
			out.z = VERTTYPEMUL(rhs.x,f[2])+VERTTYPEMUL(rhs.y,f[6])+VERTTYPEMUL(rhs.z,f[10])+VERTTYPEMUL(rhs.w,f[14]);
			out.w = VERTTYPEMUL(rhs.x,f[3])+VERTTYPEMUL(rhs.y,f[7])+VERTTYPEMUL(rhs.z,f[11])+VERTTYPEMUL(rhs.w,f[15]);

			return out;
		}

/*!***************************************************************************
 @Function			inverse
 @Returns			inverse mat4
 @Description		Calculates multiplicative inverse of this matrix
*****************************************************************************/
		Mat4 inverse() const
		{
			Mat4 out;
			PVRTMatrixInverse(out,*this);
			return out;
		}

/*!***************************************************************************
 @Function			transpose
 @Returns			transpose mat4
 @Description		Calculates transpose of this matrix
*****************************************************************************/
		Mat4 transpose() const
		{
			Mat4 out;
			out.f[0] = f[0];		out.f[1] = f[4];		out.f[2] = f[8];		out.f[3] = f[12];
			out.f[4] = f[1];		out.f[5] = f[5];		out.f[6] = f[9];		out.f[7] = f[13];
			out.f[8] = f[2];		out.f[9] = f[6];		out.f[10] = f[10];		out.f[11] = f[14];
			out.f[12] = f[3];		out.f[13] = f[7];		out.f[14] = f[11];		out.f[15] = f[15];
			return out;
		}

/*!***************************************************************************
 @Function			postTranslate
 @Input				tx distance of translation in x axis
 @Input				ty distance of translation in y axis
 @Input				tz distance of translation in z axis
 @Returns			Returns the translation defined by the passed parameters
 @Description		Translates the matrix from the passed parameters
*****************************************************************************/
		Mat4& postTranslate(VERTTYPE tx, VERTTYPE ty, VERTTYPE tz)
		{
			f[12] += VERTTYPEMUL(tx,f[0])+VERTTYPEMUL(ty,f[4])+VERTTYPEMUL(tz,f[8]);
			f[13] += VERTTYPEMUL(tx,f[1])+VERTTYPEMUL(ty,f[5])+VERTTYPEMUL(tz,f[9]);
			f[14] += VERTTYPEMUL(tx,f[2])+VERTTYPEMUL(ty,f[6])+VERTTYPEMUL(tz,f[10]);
			f[15] += VERTTYPEMUL(tx,f[3])+VERTTYPEMUL(ty,f[7])+VERTTYPEMUL(tz,f[11]);

//			col(3) += tx * col(0) + ty * col(1) + tz * col(2);
			return *this;
		}

/*!***************************************************************************
 @Function			postTranslate
 @Input				tvec translation vector
 @Returns			Returns the translation defined by the passed parameters
 @Description		Translates the matrix from the passed parameters
*****************************************************************************/
		Mat4& postTranslate(const Vec3& tvec)
		{
			return postTranslate(tvec.x, tvec.y, tvec.z);
		}

/*!***************************************************************************
 @Function			postTranslate
 @Input				tx distance of translation in x axis
 @Input				ty distance of translation in y axis
 @Input				tz distance of translation in z axis
 @Returns			Returns the translation defined by the passed parameters
 @Description		Translates the matrix from the passed parameters
*****************************************************************************/
		Mat4& preTranslate(VERTTYPE tx, VERTTYPE ty, VERTTYPE tz)
		{
			f[0]+=VERTTYPEMUL(f[3],tx);	f[4]+=VERTTYPEMUL(f[7],tx);	f[8]+=VERTTYPEMUL(f[11],tx);	f[12]+=VERTTYPEMUL(f[15],tx);
			f[1]+=VERTTYPEMUL(f[3],ty);	f[5]+=VERTTYPEMUL(f[7],ty);	f[9]+=VERTTYPEMUL(f[11],ty);	f[13]+=VERTTYPEMUL(f[15],ty);
			f[2]+=VERTTYPEMUL(f[3],tz);	f[6]+=VERTTYPEMUL(f[7],tz);	f[10]+=VERTTYPEMUL(f[11],tz);	f[14]+=VERTTYPEMUL(f[15],tz);

//			row(0) += tx * row(3);
//			row(1) += ty * row(3);
//			row(2) += tz * row(3);
			return *this;
		}

/*!***************************************************************************
 @Function			postTranslate
 @Input				tvec translation vector
 @Returns			Returns the translation defined by the passed parameters
 @Description		Translates the matrix from the passed parameters
*****************************************************************************/
		Mat4& preTranslate(const Vec3& tvec)
		{
			return preTranslate(tvec.x, tvec.y, tvec.z);
		}
/*!***************************************************************************
 @Function			ptr
 @Returns			pointer to an array of the elements of this Mat4
 @Description		Calculates transpose of this matrix
*****************************************************************************/
		VERTTYPE *ptr() { return (VERTTYPE*)&f; }

/*!***************************************************************************
** Static factory functions
*****************************************************************************/
/*!***************************************************************************
 @Function			Identity
 @Returns			a Mat4 representation of the 4x4 identity matrix
 @Description		Generates an identity matrix
*****************************************************************************/
		static Mat4 Identity()
		{
			Mat4 out;
			out.f[0] = 1;out.f[1] = 0;out.f[2] = 0;out.f[3] = 0;
			out.f[4] = 0;out.f[5] = 1;out.f[6] = 0;out.f[7] = 0;
			out.f[8] = 0;out.f[9] = 0;out.f[10] = 1;out.f[11] = 0;
			out.f[12] = 0;out.f[13] = 0;out.f[14] = 0;out.f[15] = 1;
			return out;
		}

/*!***************************************************************************
 @Function			RotationX
 @Returns			a Mat3 corresponding to the requested rotation
 @Description		Calculates a matrix corresponding to a rotation of angle
					degrees about the X axis
*****************************************************************************/
		static Mat4 RotationX(VERTTYPE angle);
/*!***************************************************************************
 @Function			RotationY
 @Returns			a Mat3 corresponding to the requested rotation
 @Description		Calculates a matrix corresponding to a rotation of angle
					degrees about the Y axis
*****************************************************************************/
		static Mat4 RotationY(VERTTYPE angle);
/*!***************************************************************************
 @Function			RotationZ
 @Returns			a Mat3 corresponding to the requested rotation
 @Description		Calculates a matrix corresponding to a rotation of angle
					degrees about the Z axis
*****************************************************************************/
		static Mat4 RotationZ(VERTTYPE angle);

/*!***************************************************************************
 @Function			Scale
 @Returns			a Mat3 corresponding to the requested scaling transformation
 @Description		Calculates a matrix corresponding to scaling of fx, fy and fz
					times in each axis.
*****************************************************************************/
		static Mat4 Scale(const VERTTYPE fx,const VERTTYPE fy,const VERTTYPE fz)
		{
			return Mat4(fx,0,0,0,
				0,fy,0,0,
				0,0,fz,0,
				0,0,0,1);
		}

/*!***************************************************************************
 @Function			Translation
 @Returns			a Mat4 corresponding to the requested translation
 @Description		Calculates a 4x4 matrix corresponding to a transformation
					of tx, ty and tz distance in each axis.
*****************************************************************************/
		static Mat4 Translation(const VERTTYPE tx, const VERTTYPE ty, const VERTTYPE tz)
		{
			return Mat4(1,0,0,0,
				0,1,0,0,
				0,0,1,0,
				tx,ty,tz,1);
		}

/*!***************************************************************************
** Clipspace enum
** Determines whether clip space Z ranges from -1 to 1 (OGL) or from 0 to 1 (D3D)
*****************************************************************************/
		enum eClipspace { OGL, D3D };

/*!***************************************************************************
 @Function			Ortho
 @Input				left view plane
 @Input				top view plane
 @Input				right view plane
 @Input				bottom view plane
 @Input				nearPlane the near rendering plane
 @Input				farPlane the far rendering plane
 @Input				cs which clipspace convention is being used
 @Input				bRotate is the viewport in portrait or landscape mode
 @Returns			Returns the orthogonal projection matrix defined by the passed parameters
 @Description		Translates the matrix from the passed parameters
*****************************************************************************/
		static Mat4 Ortho(VERTTYPE left, VERTTYPE top, VERTTYPE right,
			VERTTYPE bottom, VERTTYPE nearPlane, VERTTYPE farPlane, const eClipspace cs, bool bRotate = false)
		{
			VERTTYPE rcplmr = VERTTYPEDIV(VERTTYPE(1),(left - right));
			VERTTYPE rcpbmt = VERTTYPEDIV(VERTTYPE(1),(bottom - top));
			VERTTYPE rcpnmf = VERTTYPEDIV(VERTTYPE(1),(nearPlane - farPlane));

			Mat4 result;

			if (bRotate)
			{
				result.f[0]=0;		result.f[4]=VERTTYPEMUL(2,rcplmr); result.f[8]=0; result.f[12]=VERTTYPEMUL(-(right+left),rcplmr);
				result.f[1]=VERTTYPEMUL(-2,rcpbmt);	result.f[9]=0;		result.f[9]=0;	result.f[13]=VERTTYPEMUL((top+bottom),rcpbmt);
			}
			else
			{
				result.f[0]=VERTTYPEMUL(-2,rcplmr);	result.f[4]=0; result.f[8]=0; result.f[12]=VERTTYPEMUL(right+left,rcplmr);
				result.f[1]=0;		result.f[5]=VERTTYPEMUL(-2,rcpbmt);	result.f[9]=0;	result.f[13]=VERTTYPEMUL((top+bottom),rcpbmt);
			}
			if (cs == D3D)
			{
				result.f[2]=0;	result.f[6]=0;	result.f[10]=-rcpnmf;	result.f[14]=VERTTYPEMUL(nearPlane,rcpnmf);
			}
			else
			{
				result.f[2]=0;	result.f[6]=0;	result.f[10]=VERTTYPEMUL(-2,rcpnmf);	result.f[14]=VERTTYPEMUL(nearPlane + farPlane,rcpnmf);
			}
			result.f[3]=0;	result.f[7]=0;	result.f[11]=0;	result.f[15]=1;

			return result;
		}


/*!***************************************************************************
 @Function			LookAtRH
 @Input				vEye position of 'camera'
 @Input				vAt target that camera points at
 @Input				vUp up vector for camera
 @Returns			Returns the view matrix defined by the passed parameters
 @Description		Create a look-at view matrix for a right hand coordinate
					system
*****************************************************************************/
		static Mat4 LookAtRH(const Vec3& vEye, const Vec3& vAt, const Vec3& vUp)
			{ return LookAt(vEye, vAt, vUp, true); }
/*!***************************************************************************
 @Function			LookAtLH
 @Input				vEye position of 'camera'
 @Input				vAt target that camera points at
 @Input				vUp up vector for camera
 @Returns			Returns the view matrix defined by the passed parameters
 @Description		Create a look-at view matrix for a left hand coordinate
					system
*****************************************************************************/
		static Mat4 LookAtLH(const Vec3& vEye, const Vec3& vAt, const Vec3& vUp)
			{ return LookAt(vEye, vAt, vUp, false);	}

/*!***************************************************************************
 @Function		PerspectiveRH
 @Input			width		width of viewplane
 @Input			height		height of viewplane
 @Input			nearPlane	near clipping distance
 @Input			farPlane	far clipping distance
 @Input			cs			cs which clipspace convention is being used
 @Input			bRotate is the viewport in portrait or landscape mode
 @Returns		Perspective matrix
 @Description	Create a perspective matrix for a right hand coordinate
				system
*****************************************************************************/
		static Mat4 PerspectiveRH(VERTTYPE width, VERTTYPE height, VERTTYPE nearPlane,
			VERTTYPE farPlane, const eClipspace cs, bool bRotate = false)
			{ return Perspective(width, height, nearPlane, farPlane, cs, true, bRotate); }

/*!***************************************************************************
 @Function		PerspectiveLH
 @Input			width		width of viewplane
 @Input			height		height of viewplane
 @Input			nearPlane	near clipping distance
 @Input			farPlane	far clipping distance
 @Input			cs			cs which clipspace convention is being used
 @Input			bRotate is the viewport in portrait or landscape mode
 @Returns		Perspective matrix
 @Description	Create a perspective matrix for a left hand coordinate
				system
*****************************************************************************/
		static Mat4 PerspectiveLH(VERTTYPE width, VERTTYPE height, VERTTYPE nearPlane, VERTTYPE farPlane, const eClipspace cs, bool bRotate = false)
			{ return Perspective(width, height, nearPlane, farPlane, cs, false, bRotate); }

/*!***************************************************************************
 @Function		PerspectiveFloatDepthRH
 @Input			width		width of viewplane
 @Input			height		height of viewplane
 @Input			nearPlane	near clipping distance
 @Input			cs			cs which clipspace convention is being used
 @Input			bRotate is the viewport in portrait or landscape mode
 @Returns		Perspective matrix
 @Description	Create a perspective matrix for a right hand coordinate
				system
*****************************************************************************/
		static Mat4 PerspectiveFloatDepthRH(VERTTYPE width, VERTTYPE height, VERTTYPE nearPlane, const eClipspace cs, bool bRotate = false)
			{ return PerspectiveFloatDepth(width, height, nearPlane, cs, true, bRotate); }

/*!***************************************************************************
 @Function		PerspectiveFloatDepthLH
 @Input			width		width of viewplane
 @Input			height		height of viewplane
 @Input			nearPlane	near clipping distance
 @Input			cs			cs which clipspace convention is being used
 @Input			bRotate is the viewport in portrait or landscape mode
 @Returns		Perspective matrix
 @Description	Create a perspective matrix for a left hand coordinate
				system
*****************************************************************************/
		static Mat4 PerspectiveFloatDepthLH(VERTTYPE width, VERTTYPE height, VERTTYPE nearPlane, const eClipspace cs, bool bRotate = false)
			{ return PerspectiveFloatDepth(width, height, nearPlane, cs, false, bRotate); }

/*!***************************************************************************
 @Function		PerspectiveFovRH
 @Input			fovy		angle of view (vertical)
 @Input			aspect		aspect ratio of view
 @Input			nearPlane	near clipping distance
 @Input			farPlane	far clipping distance
 @Input			cs			cs which clipspace convention is being used
 @Input			bRotate is the viewport in portrait or landscape mode
 @Returns		Perspective matrix
 @Description	Create a perspective matrix for a right hand coordinate
				system
*****************************************************************************/
		static Mat4 PerspectiveFovRH(VERTTYPE fovy, VERTTYPE aspect, VERTTYPE nearPlane, VERTTYPE farPlane, const eClipspace cs, bool bRotate = false)
			{ return PerspectiveFov(fovy, aspect, nearPlane, farPlane, cs, true, bRotate); }
/*!***************************************************************************
 @Function		PerspectiveFovLH
 @Input			fovy		angle of view (vertical)
 @Input			aspect		aspect ratio of view
 @Input			nearPlane	near clipping distance
 @Input			farPlane	far clipping distance
 @Input			cs			cs which clipspace convention is being used
 @Input			bRotate is the viewport in portrait or landscape mode
 @Returns		Perspective matrix
 @Description	Create a perspective matrix for a left hand coordinate
				system
*****************************************************************************/
		static Mat4 PerspectiveFovLH(VERTTYPE fovy, VERTTYPE aspect, VERTTYPE nearPlane, VERTTYPE farPlane, const eClipspace cs, bool bRotate = false)
			{ return PerspectiveFov(fovy, aspect, nearPlane, farPlane, cs, false, bRotate); }

/*!***************************************************************************
 @Function		PerspectiveFovRH
 @Input			fovy		angle of view (vertical)
 @Input			aspect		aspect ratio of view
 @Input			nearPlane	near clipping distance
 @Input			cs			cs which clipspace convention is being used
 @Input			bRotate is the viewport in portrait or landscape mode
 @Returns		Perspective matrix
 @Description	Create a perspective matrix for a right hand coordinate
				system
*****************************************************************************/
		static Mat4 PerspectiveFovFloatDepthRH(VERTTYPE fovy, VERTTYPE aspect, VERTTYPE nearPlane, const eClipspace cs, bool bRotate = false)
			{ return PerspectiveFovFloatDepth(fovy, aspect, nearPlane, cs, true, bRotate); }
/*!***************************************************************************
 @Function		PerspectiveFovLH
 @Input			fovy		angle of view (vertical)
 @Input			aspect		aspect ratio of view
 @Input			nearPlane	near clipping distance
 @Input			cs			cs which clipspace convention is being used
 @Input			bRotate is the viewport in portrait or landscape mode
 @Returns		Perspective matrix
 @Description	Create a perspective matrix for a left hand coordinate
				system
*****************************************************************************/
		static Mat4 PerspectiveFovFloatDepthLH(VERTTYPE fovy, VERTTYPE aspect, VERTTYPE nearPlane, const eClipspace cs, bool bRotate = false)
			{ return PerspectiveFovFloatDepth(fovy, aspect, nearPlane, cs, false, bRotate); }

/*!***************************************************************************
 @Function			LookAt
 @Input				vEye position of 'camera'
 @Input				vAt target that camera points at
 @Input				vUp up vector for camera
 @Input				bRightHanded handedness of coordinate system
 @Returns			Returns the view matrix defined by the passed parameters
 @Description		Create a look-at view matrix
*****************************************************************************/
		static Mat4 LookAt(const Vec3& vEye, const Vec3& vAt, const Vec3& vUp, bool bRightHanded)
		{
			Vec3 vForward, vUpNorm, vSide;
			Mat4 result;

			vForward = (bRightHanded) ? vEye - vAt : vAt - vEye;

			vForward.normalize();
			vUpNorm = vUp.normalized();
			vSide   = vUpNorm.cross( vForward);
			vUpNorm = vForward.cross(vSide);

			result.f[0]=vSide.x;	result.f[4]=vSide.y;	result.f[8]=vSide.z;		result.f[12]=0;
			result.f[1]=vUpNorm.x;	result.f[5]=vUpNorm.y;	result.f[9]=vUpNorm.z;		result.f[13]=0;
			result.f[2]=vForward.x; result.f[6]=vForward.y;	result.f[10]=vForward.z;	result.f[14]=0;
			result.f[3]=0;			result.f[7]=0;			result.f[11]=0;				result.f[15]=1;

			// TODO
			result.postTranslate(-vEye);
			return result;
		}

/*!***************************************************************************
 @Function		Perspective
 @Input			width		width of viewplane
 @Input			height		height of viewplane
 @Input			nearPlane	near clipping distance
 @Input			farPlane	far clipping distance
 @Input			cs			cs which clipspace convention is being used
 @Input			bRightHanded	handedness of coordinate system
 @Input			bRotate		is the viewport in portrait or landscape mode
 @Returns		Perspective matrix
 @Description	Create a perspective matrix
*****************************************************************************/
		static Mat4 Perspective(
			VERTTYPE width, VERTTYPE height,
			VERTTYPE nearPlane, VERTTYPE farPlane,
			const eClipspace cs,
			bool bRightHanded,
			bool bRotate = false)
		{
			VERTTYPE n2 = VERTTYPEMUL(2,nearPlane);
			VERTTYPE rcpnmf = VERTTYPEDIV(1,(nearPlane - farPlane));

			Mat4 result;
			if (bRotate)
			{
				result.f[0] = 0;	result.f[4]=VERTTYPEDIV(-n2,width);	result.f[8]=0;	result.f[12]=0;
				result.f[1] = VERTTYPEDIV(n2,height);	result.f[5]=0;	result.f[9]=0;	result.f[13]=0;
			}
			else
			{
				result.f[0] = VERTTYPEDIV(n2,width);	result.f[4]=0;	result.f[8]=0;	result.f[12]=0;
				result.f[1] = 0;	result.f[5]=VERTTYPEDIV(n2,height);	result.f[9]=0;	result.f[13]=0;
			}
			if (cs == D3D)
			{
				result.f[2] = 0;	result.f[6]=0;	result.f[10]=VERTTYPEMUL(farPlane,rcpnmf);	result.f[14]=VERTTYPEMUL(VERTTYPEMUL(farPlane,rcpnmf),nearPlane);
			}
			else
			{
				result.f[2] = 0;	result.f[6]=0;	result.f[10]=VERTTYPEMUL(farPlane+nearPlane,rcpnmf);	result.f[14]=VERTTYPEMUL(VERTTYPEMUL(farPlane,rcpnmf),n2);
			}
			result.f[3] = 0;	result.f[7]=0;	result.f[11]=-1;	result.f[15]=0;

			if (!bRightHanded)
			{
				result.f[10] *= -1;
				result.f[11] = 1;
			}

			return result;
		}

/*!***************************************************************************
 @Function		Perspective
 @Input			width		width of viewplane
 @Input			height		height of viewplane
 @Input			nearPlane	near clipping distance
 @Input			cs			cs which clipspace convention is being used
 @Input			bRightHanded	handedness of coordinate system
 @Input			bRotate		is the viewport in portrait or landscape mode
 @Returns		Perspective matrix
 @Description	perspective calculation where far plane is assumed to be at an infinite distance and the screen
				space Z is inverted
*****************************************************************************/
		static Mat4 PerspectiveFloatDepth(
			VERTTYPE width, VERTTYPE height,
			VERTTYPE nearPlane,
			const eClipspace cs,
			bool bRightHanded,
			bool bRotate = false)
		{
			VERTTYPE n2 = VERTTYPEMUL(2,nearPlane);

			Mat4 result;
			if (bRotate)
			{
				result.f[0] = 0;	result.f[4]=VERTTYPEDIV(-n2,width);	result.f[8]=0;	result.f[12]=0;
				result.f[1] = VERTTYPEDIV(n2,height);	result.f[5]=0;	result.f[9]=0;	result.f[13]=0;
			}
			else
			{
				result.f[0] = VERTTYPEDIV(n2,width);	result.f[4]=0;	result.f[8]=0;	result.f[12]=0;
				result.f[1] = 0;	result.f[5]=VERTTYPEDIV(n2,height);	result.f[9]=0;	result.f[13]=0;
			}
			if (cs == D3D)
			{
				result.f[2] = 0;	result.f[6]=0;	result.f[10]=0;	result.f[14]=nearPlane;
			}
			else
			{
				result.f[2] = 0;	result.f[6]=0;	result.f[10]=(bRightHanded?(VERTTYPE)1:(VERTTYPE)-1);	result.f[14]=n2;
			}
			result.f[3] = (VERTTYPE)0;	result.f[7]=(VERTTYPE)0;	result.f[11]= (bRightHanded?(VERTTYPE)-1:(VERTTYPE)1);	result.f[15]=(VERTTYPE)0;

			return result;
		}

/*!***************************************************************************
 @Function		Perspective
 @Input			fovy		angle of view (vertical)
 @Input			aspect		aspect ratio of view
 @Input			nearPlane	near clipping distance
 @Input			farPlane	far clipping distance
 @Input			cs			cs which clipspace convention is being used
 @Input			bRightHanded	handedness of coordinate system
 @Input			bRotate		is the viewport in portrait or landscape mode
 @Returns		Perspective matrix
 @Description	perspective calculation where field of view is used instead of near plane dimensions
*****************************************************************************/
		static Mat4 PerspectiveFov(
			VERTTYPE fovy, VERTTYPE aspect,
			VERTTYPE nearPlane, VERTTYPE farPlane,
			const eClipspace cs,
			bool bRightHanded,
			bool bRotate = false)
		{
			VERTTYPE height = VERTTYPEMUL(VERTTYPEMUL(2,nearPlane),PVRTTAN(VERTTYPEMUL(fovy,0.5f)));
			if (bRotate) return Perspective(height, VERTTYPEDIV(height,aspect), nearPlane, farPlane, cs, bRightHanded, bRotate);
			return Perspective(VERTTYPEMUL(height,aspect), height, nearPlane, farPlane, cs, bRightHanded, bRotate);
		}

/*!***************************************************************************
 @Function		Perspective
 @Input			fovy		angle of view (vertical)
 @Input			aspect		aspect ratio of view
 @Input			nearPlane	near clipping distance
 @Input			cs			cs which clipspace convention is being used
 @Input			bRightHanded	handedness of coordinate system
 @Input			bRotate		is the viewport in portrait or landscape mode
 @Returns		Perspective matrix
 @Description	perspective calculation where field of view is used instead of near plane dimensions
				and far plane is assumed to be at an infinite distance with inverted Z range
*****************************************************************************/
		static Mat4 PerspectiveFovFloatDepth(
			VERTTYPE fovy, VERTTYPE aspect,
			VERTTYPE nearPlane,
			const eClipspace cs,
			bool bRightHanded,
			bool bRotate = false)
		{
			VERTTYPE height = VERTTYPEMUL(VERTTYPEMUL(2,nearPlane), PVRTTAN(VERTTYPEMUL(fovy,0.5)));
			if (bRotate) return PerspectiveFloatDepth(height, VERTTYPEDIV(height,aspect), nearPlane, cs, bRightHanded, bRotate);
			return PerspectiveFloatDepth(VERTTYPEMUL(height,aspect), height, nearPlane, cs, bRightHanded, bRotate);
		}
	};

}

#endif /*__PVRTVECTOR_H__*/

/*****************************************************************************
End of file (PVRTVector.h)
*****************************************************************************/
