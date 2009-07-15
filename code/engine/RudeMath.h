/*
 *  RudeMath.h
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef __H_RudeMath
#define __H_RudeMath

#include "RudeGlobals.h"
#include "RudeDebug.h"

#ifdef RUDE_WIN_DESK
#include <float.h>
#endif

#ifdef RUDE_PALM
#include "MathLib.h"
#include <SysUtils.h>
#define FASTINVSQRT
#define abs Abs
#endif

#ifdef RUDE_GAPI
#include <math.h>
#endif

#ifdef RUDE_SYMBIAN
#include <math.h>
#include <stdlib.h>
#endif

#ifdef RUDE_IPHONE
#include <math.h>
#include <stdlib.h>
#endif

class RudeMath {

public:

	static void startup();

	static void EnableFPE();
	
	static int Rand()
	{
#ifdef RUDE_GAPI
		return rand();
#endif

#ifdef RUDE_PALM
		return SysRandom(0);
#endif

#ifdef RUDE_SYMBIAN
		return rand();
#endif
		
#ifdef RUDE_IPHONE
		return rand();
#endif
	}

	static float InvSqrt(float x)
	{
#ifdef FASTINVSQRT
		float xhalf = 0.5f * x;
		long i = *(long*)&x;
		i = 0x5f3759df - (i >> 1);
		x = *(float*)&i;
		x = x*(1.5f - xhalf*x*x);

		return x;
#else

#ifdef RUDE_PALM
		double result;
		MathLibSqrt(MathLibRef, x, &result);
		return (float) 1.0f / result;
#else
		return 1.0f / (float) sqrt(x);
#endif

#endif
	}
	
	static float FRand(float max)
	{
		float r = ((float) rand()) / ((float) RAND_MAX);
		return r * max;
	}
	
	static float Sqrt(float x)
	{
		return (float) sqrt(x);
	}
	
	static float Cos(float x)
	{
#ifdef RUDE_PALM
		double result;
		MathLibCos(MathLibRef, x, &result);
		return (float) result;
#else
		return (float) cos(x);
#endif

	}
	
	static float Sin(float x)
	{
#ifdef RUDE_PALM
		double result;
		MathLibSin(MathLibRef, x, &result);
		return (float) result;
#else
		return (float) sin(x);
#endif

	}
	
	static float ATan2f(float x, float y)
	{
#ifdef RUDE_PALM
		double result;
		MathLibATan2(MathLibRef, x, y, &result);
		return result;
#endif

#ifdef RUDE_SYMBIAN
		return atan2(x, y);
#endif

#ifdef RUDE_GAPI
		return (float) atan2(x, y);
#endif
		
#ifdef RUDE_IPHONE
		return atan2(x, y);
#endif
	}

private:
	RudeMath();
	~RudeMath();

};

#endif
