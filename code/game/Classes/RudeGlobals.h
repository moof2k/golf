#ifndef _H_RudeGlobals
#define _H_RudeGlobals

#include "Rude.h"

#include "RudeColor.h"


#define kRudeGlobalsAlphaTableLength 257
#define kRudeGlobalsSinTableLength 360
#define kRudeGlobalsCosTableLength 360

#ifdef RUDE_SYMBIAN
static const TUid kRudeGlobalsUid = { KUidVectorBlaster+2 };
class RudeGlobals : public CCoeStatic
#else
class RudeGlobals
#endif
{
public:
#ifdef RUDE_SYMBIAN
	RudeGlobals(TUid uid);
#else
	RudeGlobals();
#endif
	~RudeGlobals();

	static RudeGlobals * GetInstance();

	int GetAlpha(int index) { return _alphatable[index]; }
	float GetSin(int index) { return _sintable[index]; }
	float GetCos(int index) { return _costable[index]; }

	static void GetPath(char *path);
	
	RudeColor* GetWhite() { return _white; }
	RudeColor* GetBlack() { return _black; }
	RudeColor* GetRed() { return _red; }
	RudeColor* GetGreen() { return _green; }
	RudeColor* GetBlue() { return _blue; }

private:

	int _alphatable[kRudeGlobalsAlphaTableLength];
	float _sintable[kRudeGlobalsSinTableLength];
	float _costable[kRudeGlobalsCosTableLength];
	
	RudeColor *_white;
	RudeColor *_black;
	RudeColor *_red;
	RudeColor *_green;
	RudeColor *_blue;
};


#endif

