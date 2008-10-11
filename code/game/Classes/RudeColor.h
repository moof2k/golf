#ifndef _H_RudeColor
#define _H_RudeColor

#include "Rude.h"

#ifdef RUDE_GAPI
#include "GapiDraw.h"
#endif

class RudeColor {

public:
	RudeColor();
	RudeColor(int r, int g, int b);
	RudeColor(const RudeColor &c);
	~RudeColor();
	
	int r;
	int g;
	int b;

	void SetColor(int r, int g, int b) { this->r = r; this->g = g; this->b = b; }
	void SetColor(RudeColor *c) { this->r = c->r; this->g = c->g; this->b = c->b; }

#ifdef RUDE_GAPI
	COLORREF GetColor() { return RGB(r, g, b); }
	void SetColor(COLORREF c) { r = GetRValue(c); g = GetGValue(c); b = GetBValue(c); }
#endif


	
private:

};


#endif