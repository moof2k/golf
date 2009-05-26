#include "RudeMath.h"



RudeMath::RudeMath()
{

}

RudeMath::~RudeMath()
{

}

void RudeMath::startup()
{
#ifdef RUDE_PALMOS
	
#endif

}

//
// RudeMath::EnableFPE()
//
// enable floating point exception handling

#ifdef RUDE_SYMBIAN
void RudeMathSymbianFPEHandler(TExcType a)
{
	/*RudeDebug *debug = RudeDebug::GetInstance();
	char str[30];
	sprintf(str, "FPE %ld", (long) a);
	debug->Write(str);*/
}
#endif

void RudeMath::EnableFPE()
{
#ifdef RUDE_WIN_DESK
	
	int cfp = _controlfp(0, 0);
	cfp &= ~(EM_ZERODIVIDE|EM_OVERFLOW|EM_INVALID);
	_controlfp(cfp, MCW_EM);
#endif

#ifdef RUDE_SYMBIAN
	RThread().SetExceptionHandler(RudeMathSymbianFPEHandler, KExceptionFpe);
#endif

}