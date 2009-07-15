/*
 *  RudeFont.h
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef __H_RudeFont
#define __H_RudeFont

#include <vector>
#include <string>
#include <map>



class CDynRender;

class CFontLoader;

struct SCharDescr
{
	SCharDescr() : srcX(0), srcY(0), srcW(0), srcH(0), xOff(0), yOff(0), xAdv(0), page(0) {}
	
	short srcX;
	short srcY;
	short srcW;
	short srcH;
	short xOff;
	short yOff;
	short xAdv;
	short page;
	unsigned int chnl;
	
	std::vector<int> kerningPairs;
};

enum EFontTextEncoding
{
	NONE,
	UTF8,
	UTF16
};

class RudeFont
{
public:
	RudeFont();
	~RudeFont();
	
	int Init(const char *fontFile);
	
	void SetTextEncoding(EFontTextEncoding encoding);
	
	float GetTextWidth(const char *text, int count);
	
	void Printf(float x, float y, float z, unsigned int mode, unsigned int topcolor, unsigned int botcolor, const char *fmt, ...);
	
	void Write(float x, float y, float z, const char *text, int count, unsigned int mode, unsigned int color);
	void Write(float x, float y, float z, const char *text, int count, unsigned int mode, unsigned int topcolor, unsigned int botcolor);
	void WriteML(float x, float y, float z, const char *text, int count, unsigned int mode);
	void WriteBox(float x, float y, float z, float width, const char *text, int count, unsigned mode);
	
	void SetHeight(float h);
	float GetHeight();
	
	float GetBottomOffset();
	float GetTopOffset();
	
	void PrepareEffect();
	void PreparePixelPerfectOutput();
	
protected:
	friend class CFontLoader;
	
	void InternalWrite(float x, float y, float z, const char *text, int count, float spacing = 0);
	
	float AdjustForKerningPairs(int first, int second);
	SCharDescr *GetChar(int id);
	
	int GetTextLength(const char *text);
	int GetTextChar(const char *text, int pos, int *nextPos = 0);
	int FindTextChar(const char *text, int start, int length, int ch);
	
	short fontHeight; // total height of the font
	short base;       // y of base line
	short scaleW;
	short scaleH;
	SCharDescr defChar;
	bool hasOutline;
	
	float scale;
	EFontTextEncoding encoding;
	
	std::map<int, SCharDescr*> chars;
	std::vector<int> pages;
};

const int FONT_ALIGN_LEFT    = 0;
const int FONT_ALIGN_CENTER  = 1;
const int FONT_ALIGN_RIGHT   = 2;
const int FONT_ALIGN_JUSTIFY = 3;

typedef enum {
	kDefaultFont = 0,
	kDefaultFontOutline,
	kBigFont,
	kBigFontOutline,
	
	kNumFonts,
	
} eFont;

class RudeFontManager {

public:
	
	static void InitFonts();
	
	static RudeFont * GetFont(eFont font);
	
private:
	
	static RudeFont m_fonts[kNumFonts];
	
};

#endif
