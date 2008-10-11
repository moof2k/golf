
#include "RudeColor.h"

RudeColor::RudeColor()
{

}

RudeColor::RudeColor(int red, int green, int blue)
{
	this->r = red;
	this->g = green;
	this->b = blue;
}

RudeColor::RudeColor(const RudeColor &c)
{
	this->r = c.r;
	this->g = c.g;
	this->b = c.b;
}


RudeColor::~RudeColor()
{
#if defined(__MARM_ARMI__)
//	__asm("and r0,sp,#0xff000");
#endif

}




