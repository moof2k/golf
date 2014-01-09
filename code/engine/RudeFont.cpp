/*
 *  RudeFont.cpp
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

/*
 AngelCode Tool Box Library
 Copyright (c) 2007-2008 Andreas Jonsson
 
 This software is provided 'as-is', without any express or implied 
 warranty. In no event will the authors be held liable for any 
 damages arising from the use of this software.
 
 Permission is granted to anyone to use this software for any 
 purpose, including commercial applications, and to alter it and 
 redistribute it freely, subject to the following restrictions:
 
 1. The origin of this software must not be misrepresented; you 
 must not claim that you wrote the original software. If you use
 this software in a product, an acknowledgment in the product 
 documentation would be appreciated but is not required.
 
 2. Altered source versions must be plainly marked as such, and 
 must not be misrepresented as being the original software.
 
 3. This notice may not be removed or altered from any source 
 distribution.
 
 
 Andreas Jonsson
 andreas@angelcode.com
 */

#include "RudeDebug.h"
#include "RudeFont.h"
#include "RudeGL.h"
#include "RudeUnicode.h"
#include "RudeFile.h"
#include "RudeTextureManager.h"


typedef unsigned int UINT;
typedef unsigned char BYTE;

using namespace std;


RudeFont RudeFontManager::m_fonts[kNumFonts];

/**
 * Initialize fonts.  Must be called before a call to GetFont(), preferably
 * at application start time.
 */
void RudeFontManager::InitFonts()
{
	if(RUDE_IPAD)
	{
		m_fonts[kDefaultFont].Init("ab28.fnt");
		m_fonts[kDefaultFontOutline].Init("ab28o2.fnt");
		m_fonts[kBigFont].Init("ab48.fnt");
		m_fonts[kBigFontOutline].Init("ab48o2.fnt");
	}
	else
	{
		m_fonts[kDefaultFont].Init("ab18.fnt");
		m_fonts[kDefaultFontOutline].Init("ab18o2.fnt");
		m_fonts[kBigFont].Init("ab28.fnt");
		m_fonts[kBigFontOutline].Init("ab28o2.fnt");
	}
}

/**
 * Returns a pointer to the given font.  Does not perform any error checking.
 */
RudeFont * RudeFontManager::GetFont(eFont f)
{
	RUDE_ASSERT(f >= 0, "Invalid font");

	return &m_fonts[f];
}


// Implement private helper classes for loading the bitmap font files

class CFontLoader
	{
	public:
		CFontLoader(FILE *f, RudeFont *font, const char *fontFile);
        virtual ~CFontLoader() {};
		
		virtual int Load() = 0; // Must be implemented by derived class
		
	protected:
		void LoadPage(int id, const char *pageFile, const char *fontFile);
		void SetFontInfo(int outlineThickness);
		void SetCommonInfo(int fontHeight, int base, int scaleW, int scaleH, int pages, bool isPacked);
		void AddChar(int id, int x, int y, int w, int h, int xoffset, int yoffset, int xadvance, int page, int chnl);
		void AddKerningPair(int first, int second, int amount);
		
		FILE *f;
		RudeFont *font;
		const char *fontFile;
		
		int outlineThickness;
	};

class CFontLoaderTextFormat : public CFontLoader
	{
	public:
		CFontLoaderTextFormat(FILE *f, RudeFont *font, const char *fontFile);
		
		int Load();
		
		int SkipWhiteSpace(std::string &str, int start);
		int FindEndOfToken(std::string &str, int start);
		
		void InterpretInfo(std::string &str, int start);
		void InterpretCommon(std::string &str, int start);
		void InterpretChar(std::string &str, int start);
		void InterpretSpacing(std::string &str, int start);
		void InterpretKerning(std::string &str, int start);
		void InterpretPage(std::string &str, int start, const char *fontFile);
	};

class CFontLoaderBinaryFormat : public CFontLoader
	{
	public:
		CFontLoaderBinaryFormat(FILE *f, RudeFont *font, const char *fontFile);
		
		int Load();
		
		void ReadInfoBlock(int size);
		void ReadCommonBlock(int size);
		void ReadPagesBlock(int size);
		void ReadCharsBlock(int size);
		void ReadKerningPairsBlock(int size);
	};


RudeFont::RudeFont()
{
	fontHeight = 0;
	base = 0;
	scaleW = 0;
	scaleH = 0;
	scale = 1.0f;
	hasOutline = false;
	encoding = UTF8;
}

RudeFont::~RudeFont()
{
	std::map<int, SCharDescr*>::iterator it = chars.begin();
	while( it != chars.end() )
	{
		delete it->second;
		it++;
	}
	
	/*
	for( UINT n = 0; n < pages.size(); n++ )
		if( pages[n] )
			pages[n]->Release();
	 */
}

/**
 * Initialize a font.  It's not necessary to use this function if you're using the RudeFontManager.
 */
int RudeFont::Init(const char *fontFileIn)
{
	char fontFile[512];
	RudeFileGetFile(fontFileIn, fontFile, 512);
	
	// Load the font
	FILE *f = fopen(fontFile, "rb");
	RUDE_ASSERT(f, "Unable to open font file %s", fontFileIn);
	
	// Determine format by reading the first bytes of the file
	char str[4] = {0};
	fread(str, 3, 1, f);
	fseek(f, 0, SEEK_SET);
	
	CFontLoader *loader = new CFontLoaderTextFormat(f, this, fontFile);
	RUDE_ASSERT(loader, "Failed to allocate font loader");
	
	int r = loader->Load();
	delete loader;
	
	return r;
}

void RudeFont::SetTextEncoding(EFontTextEncoding encoding)
{
	this->encoding = encoding;
}

SCharDescr *RudeFont::GetChar(int id)
{
	std::map<int, SCharDescr*>::iterator it = chars.find(id);
	if( it == chars.end() ) return 0;
	
	return it->second;
}

float RudeFont::AdjustForKerningPairs(int first, int second)
{	
	SCharDescr *ch = GetChar(first);
	if( ch == 0 ) return 0;
	for( UINT n = 0; n < ch->kerningPairs.size(); n += 2 )
	{
		if( ch->kerningPairs[n] == second )
			return ch->kerningPairs[n+1] * scale;
	}
	
	return 0;
}

float RudeFont::GetTextWidth(const char *text, int count)
{
	if( count <= 0 )
		count = GetTextLength(text);
	
	float x = 0;
	
	for( int n = 0; n < count; )
	{
		int charId = GetTextChar(text,n,&n);
		
		SCharDescr *ch = GetChar(charId);
		if( ch == 0 ) ch = &defChar;
		
		x += scale * (ch->xAdv);
		
		if( n < count )
			x += AdjustForKerningPairs(charId, GetTextChar(text,n));
	}
	
	return x;
}

void RudeFont::SetHeight(float h)
{
	scale = h / float(fontHeight);
}

float RudeFont::GetHeight()
{
	return scale * float(fontHeight);
}

float RudeFont::GetBottomOffset()
{
	return scale * (base - fontHeight);
}

float RudeFont::GetTopOffset()
{
	return scale * (base - 0);
}

/**
 * Returns the number of bytes in the string until the null char
 */
int RudeFont::GetTextLength(const char *text)
{
	if( encoding == UTF16 )
	{
		int textLen = 0;
		for(;;)
		{
			unsigned int len;
			int r = DecodeUTF16(&text[textLen], &len);
			if( r > 0 )
				textLen += len;
			else if( r < 0 )
				textLen++;
			else
				return textLen;
		}
	}
	
	// Both UTF8 and standard ASCII strings can use strlen
	return (int)strlen(text);
}

int RudeFont::GetTextChar(const char *text, int pos, int *nextPos)
{
	int ch;
	unsigned int len;
	if( encoding == UTF8 )
	{
		ch = DecodeUTF8(&text[pos], &len);
		if( ch == -1 ) len = 1;
	}
	else if( encoding == UTF16 )
	{
		ch = DecodeUTF16(&text[pos], &len);
		if( ch == -1 ) len = 2;
	}
	else
	{
		len = 1;
		ch = (unsigned char)text[pos];
	}
	
	if( nextPos ) *nextPos = pos + len;
	return ch;
}

int RudeFont::FindTextChar(const char *text, int start, int length, int ch)
{
	int pos = start;
	int nextPos;
	int currChar = -1;
	while( pos < length )
	{
		currChar = GetTextChar(text, pos, &nextPos);
		if( currChar == ch )
			return pos;
		pos = nextPos;
	} 
	
	return -1;
}

void RudeFont::InternalWrite(float x, float y, float z, const char *text, int count, float spacing)
{	
	int page = -1;
	
	y -= scale * float(base);
	
	for( int n = 0; n < count; )
	{
		//char c = text[n];
		
		int charId = GetTextChar(text, n, &n);
		SCharDescr *ch = GetChar(charId);
		if( ch == 0 ) ch = &defChar;
		
		// Map the center of the texel to the corners
		// in order to get pixel perfect mapping
		float u = (float(ch->srcX)) / scaleW;
		float v = (float(ch->srcY)) / scaleH;
		float u2 = u + float(ch->srcW) / scaleW;
		float v2 = v + float(ch->srcH) / scaleH;
		
		v = 1.0f - v;
		v2 = 1.0f - v2;
		
		float a = scale * float(ch->xAdv);
		float w = scale * float(ch->srcW);
		float h = -scale * float(ch->srcH);
		float ox = scale * float(ch->xOff);
		float oy = scale * float(ch->yOff);
		
		if( ch->page != page )
		{
			page = ch->page;
			RudeTextureManager::GetInstance()->SetTexture(pages[page]);
			
		}
		
		GLfloat point[] = {
			x+ox, y+oy, z,
			x+w+ox, y+oy, z,
			x+w+ox, y-h+oy, z,
			x+ox, y-h+oy, z
		};
		
		GLfloat uvs[] = {
			u, v, 
			u2, v,
			u2, v2,
			u, v2,
		};
		
		glVertexPointer(3, GL_FLOAT, 0, point);
		glTexCoordPointer(2, GL_FLOAT, 0, uvs);
		
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		
		x += a;
		if( charId == ' ' )
			x += spacing;
		
		if( n < count )
			x += AdjustForKerningPairs(charId, GetTextChar(text,n));
		
		//printf("c=%c a=%f x=%f\n", c, a, x);
	}
	
	 
}

/**
 * Print a formatted string to the screen at the given x,y,z location
 */
void RudeFont::Printf(float x, float y, float z, unsigned int mode, unsigned int topcolor, unsigned int botcolor, const char *pszFormat, ...)
{
	va_list args;
	char text[512];
	
	va_start(args, pszFormat);
	vsprintf(text, pszFormat, args);
	va_end(args);
	
	Write(x, y, z, text, 0, mode, topcolor, botcolor);
	
}

/**
 * Write a string to the screen at the given x,y,z location
 */
void RudeFont::Write(float x, float y, float z, const char *text, int count, unsigned int mode, unsigned int color)
{
	Write(x, y, z, text, count, mode, color, color);
}

/**
 * Write a string to the screen at the given x,y,z location.  Additionally adds a vertical transition between two colors.
 * Useful for shadow effects or making text "pop".
 */
void RudeFont::Write(float x, float y, float z, const char *text, int count, unsigned int mode, unsigned int topcolor, unsigned int botcolor)
{
	RGL.EnableClient(kVertexArray, true);
	RGL.EnableClient(kColorArray, true);
	RGL.EnableClient(kTextureCoordArray, true);
	
	unsigned int colors[] = {
		topcolor,
		topcolor,
		botcolor,
		botcolor
	};
	
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, colors);
	
	if( count <= 0 )
		count = GetTextLength(text);
	
	if( mode == FONT_ALIGN_CENTER )
	{
		float w = GetTextWidth(text, count);
		x -= w/2;
	}
	else if( mode == FONT_ALIGN_RIGHT )
	{
		float w = GetTextWidth(text, count);
		x -= w;
	}
	
	InternalWrite(x, y, z, text, count);
}

/**
 * Write a multi-line block of text at the given x,y,z location
 */
void RudeFont::WriteML(float x, float y, float z, const char *text, int count, unsigned int mode)
{
	if( count <= 0 )
		count = GetTextLength(text);
	
	// Get first line
	int pos = 0;
	int len = FindTextChar(text, pos, count, '\n'); 
	if( len == -1 ) len = count;
	while( pos < count )
	{
		float cx = x;
		if( mode == FONT_ALIGN_CENTER )
		{
			float w = GetTextWidth(&text[pos], len);
			cx -= w/2;
		}
		else if( mode == FONT_ALIGN_RIGHT )
		{
			float w = GetTextWidth(&text[pos], len);
			cx -= w;
		}
		
		InternalWrite(cx, y, z, &text[pos], len);
		
		y += scale * float(fontHeight);
		
		// Get next line
		pos += len;
		int ch = GetTextChar(text, pos, &pos);
		if( ch == '\n' )
		{
			len = FindTextChar(text, pos, count, '\n');
			if( len == -1 )
				len = count - pos;
			else 
				len = len - pos;
		}
	}
}

/**
 * Write a block of text to the screen, but keep it within the given width
 */
void RudeFont::WriteBox(float x, float y, float z, float width, const char *text, int count, unsigned int mode)
{
	if( count <= 0 )
		count = GetTextLength(text);
	
	float currWidth = 0, wordWidth;
	int lineS = 0, lineE = 0, wordS = 0, wordE = 0;
	int wordCount = 0;
	
	const char *s = " ";
	float spaceWidth = GetTextWidth(s, 1);
	bool softBreak = false;
	
	for(; lineS < count;)
	{
		// Determine the extent of the line
		for(;;)
		{
			// Determine the number of characters in the word
			while( wordE < count && 
				  GetTextChar(text,wordE) != ' ' &&
				  GetTextChar(text,wordE) != '\n' )
				// Advance the cursor to the next character
				GetTextChar(text,wordE,&wordE);
			
			// Determine the width of the word
			if( wordE > wordS )
			{
				wordCount++;
				wordWidth = GetTextWidth(&text[wordS], wordE-wordS);
			}
			else
				wordWidth = 0;
			
			// Does the word fit on the line? The first word is always accepted.
			if( wordCount == 1 || currWidth + (wordCount > 1 ? spaceWidth : 0) + wordWidth <= width )
			{
				// Increase the line extent to the end of the word
				lineE = wordE;
				currWidth += (wordCount > 1 ? spaceWidth : 0) + wordWidth;
				
				// Did we reach the end of the line?
				if( wordE == count || GetTextChar(text,wordE) == '\n' )
				{
					softBreak = false;
					
					// Skip the newline character
					if( wordE < count )
						// Advance the cursor to the next character
						GetTextChar(text,wordE,&wordE);
					
					break;
				}				
				
				// Skip the trailing space
				if( wordE < count && GetTextChar(text,wordE) == ' ' )
					// Advance the cursor to the next character
					GetTextChar(text,wordE,&wordE);
				
				// Move to next word
				wordS = wordE;
			}
			else
			{
				softBreak = true;
				
				// Skip the trailing space
				if( wordE < count && GetTextChar(text,wordE) == ' ' )
					// Advance the cursor to the next character
					GetTextChar(text,wordE,&wordE);
				
				break;
			}
		}
		
		// Write the line
		if( mode == FONT_ALIGN_JUSTIFY )
		{
			float spacing = 0;
			if( softBreak )
			{
				if( wordCount > 2 )
					spacing = (width - currWidth) / (wordCount-2);
				else
					spacing = (width - currWidth);
			}
			
			InternalWrite(x, y, z, &text[lineS], lineE - lineS, spacing);
		}
		else
		{
			float cx = x;
			if( mode == FONT_ALIGN_RIGHT )
				cx = x + width - currWidth;
			else if( mode == FONT_ALIGN_CENTER )
				cx = x + 0.5f*(width - currWidth);
			
			InternalWrite(cx, y, z, &text[lineS], lineE - lineS);
		}
		
		if( softBreak )
		{
			// Skip the trailing space
			if( lineE < count && GetTextChar(text,lineE) == ' ' )
				// Advance the cursor to the next character
				GetTextChar(text,lineE,&lineE);
			
			// We've already counted the first word on the next line
			currWidth = wordWidth;
			wordCount = 1;
		}
		else
		{
			// Skip the line break
			if( lineE < count && GetTextChar(text,lineE) == '\n' )
				// Advance the cursor to the next character
				GetTextChar(text,lineE,&lineE);
			
			currWidth = 0;
			wordCount = 0;
		}
		
		// Move to next line
		lineS = lineE;
		wordS = wordE;
		y -= scale * float(fontHeight);
	}
}

void RudeFont::PrepareEffect()
{

}

/**
 * Has no effect.  For pixel-perfect output use orthgraphic rendering.
 */
void RudeFont::PreparePixelPerfectOutput()
{
	/*
	IDirect3DDevice9 *dev = render->GetDevice();
	CGraphics *gfx = render->GetGraphics();
	
	// Determine size of view port
	D3DVIEWPORT9 vp;
	dev->GetViewport(&vp);
	
	// Clear world matrix
	D3DXMATRIX mtx;
	D3DXMatrixIdentity(&mtx);
	gfx->SetMatrix(D3DTS_WORLD, &mtx);
	
	// Setup orthogonal view
	// Origin is in lower-left corner
	D3DXMatrixOrthoOffCenterLH(&mtx, 0, (float)vp.Width, 0, (float)vp.Height, vp.MinZ, vp.MaxZ);
	gfx->SetMatrix(D3DTS_VIEW, &mtx);
	
	D3DXMatrixIdentity(&mtx);
	gfx->SetMatrix(D3DTS_PROJECTION, &mtx);
	 
	 */
	
	// Adjust the scale of the font so that the 
	// resolution of texture is the same as the screen
	scale = 1.0f;
	 
}

//=============================================================================
// CFontLoader
//
// This is the base class for all loader classes. This is the only class
// that has access to and knows how to set the CFont members.
//=============================================================================

CFontLoader::CFontLoader(FILE *f, RudeFont *font, const char *fontFile)
{
	this->f = f;
	this->font = font;
	this->fontFile = fontFile;
	
	outlineThickness = 0;
}

void CFontLoader::LoadPage(int id, const char *pageFile, const char *fontFile)
{
	/*
	string str;
	
	// Load the texture from the same directory as the font descriptor file
	
	// Find the directory
	str = fontFile;
	for( size_t n = 0; (n = str.find('/', n)) != string::npos; ) str.replace(n, 1, "\\");
	size_t i = str.rfind('\\');
	if( i != string::npos )
		str = str.substr(0, i+1);
	else
		str = "";
	 */
	
	char pageFile2[512];
	
	for(unsigned int i = 0; i < strlen(pageFile); i++)
	{
		pageFile2[i] = pageFile[i];
		
		if(pageFile2[i] == '.')
			pageFile2[i] = '\0';
	}
	
	// Load the font textures
	//str += pageFile2;
	
	font->pages[id] = RudeTextureManager::GetInstance()->LoadTextureFromPVRTFile(pageFile2);
	
	
	/*
	IDirect3DTexture9 *texture = 0;
	UINT mipLevels = 0; // 0 = all
	HRESULT hr = D3DXCreateTextureFromFileEx(font->render->GetDevice(), str.c_str(), 
											 D3DX_DEFAULT, D3DX_DEFAULT, mipLevels, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, 
											 D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &texture);
	if( FAILED(hr) )
	{
		LOG(("Failed to load font page '%s' (%X)", str.c_str(), hr));
	}
	font->pages[id] = texture;
	 */
}

void CFontLoader::SetFontInfo(int outlineThickness)
{
	this->outlineThickness = outlineThickness;
}

void CFontLoader::SetCommonInfo(int fontHeight, int base, int scaleW, int scaleH, int pages, bool isPacked)
{
	font->fontHeight = fontHeight;
	font->base = base;
	font->scaleW = scaleW;
	font->scaleH = scaleH;
	font->pages.resize(pages);
	for( int n = 0; n < pages; n++ )
		font->pages[n] = 0;
	
	if( isPacked && outlineThickness )
		font->hasOutline = true;
}

void CFontLoader::AddChar(int id, int x, int y, int w, int h, int xoffset, int yoffset, int xadvance, int page, int chnl)
{
	// Convert to a 4 element vector
	// TODO: Does this depend on hardware? It probably does
	if     ( chnl == 1 ) chnl = 0x00010000;  // Blue channel
	else if( chnl == 2 ) chnl = 0x00000100;  // Green channel
	else if( chnl == 4 ) chnl = 0x00000001;  // Red channel
	else if( chnl == 8 ) chnl = 0x01000000;  // Alpha channel
	else chnl = 0;
	
	if( id >= 0 )
	{
		SCharDescr *ch = new SCharDescr;
		ch->srcX = x;
		ch->srcY = y;
		ch->srcW = w;
		ch->srcH = h;
		ch->xOff = xoffset;
		ch->yOff = yoffset;
		ch->xAdv = xadvance;
		ch->page = page;
		ch->chnl = chnl;
		
		font->chars.insert(std::map<int, SCharDescr*>::value_type(id, ch));
	}
	
	if( id == -1 )
	{
		font->defChar.srcX = x;
		font->defChar.srcY = y;
		font->defChar.srcW = w;
		font->defChar.srcH = h;
		font->defChar.xOff = xoffset;
		font->defChar.yOff = yoffset;
		font->defChar.xAdv = xadvance;
		font->defChar.page = page;
		font->defChar.chnl = chnl;
	}
}

void CFontLoader::AddKerningPair(int first, int second, int amount)
{
	if( first >= 0 && first < 256 && font->chars[first] )
	{
		font->chars[first]->kerningPairs.push_back(second);
		font->chars[first]->kerningPairs.push_back(amount);
	}
}

//=============================================================================
// CFontLoaderTextFormat
//
// This class implements the logic for loading a BMFont file in text format
//=============================================================================

CFontLoaderTextFormat::CFontLoaderTextFormat(FILE *f, RudeFont *font, const char *fontFile) : CFontLoader(f, font, fontFile)
{
}

int CFontLoaderTextFormat::Load()
{
	string line;
	
	while( !feof(f) )
	{
		// Read until line feed (or EOF)
		line = "";
		line.reserve(256);
		while( !feof(f) )
		{
			char ch;
			if( fread(&ch, 1, 1, f) )
			{
				if( ch != '\n' ) 
					line += ch; 
				else
					break;
			}
		}
		
		// Skip white spaces
		int pos = SkipWhiteSpace(line, 0);
		if( pos >= line.size() ) break;
		
		// Read token
		int pos2 = FindEndOfToken(line, pos);
		string token = line.substr(pos, pos2-pos);
		
		// Interpret line
		if( token == "info" )
			InterpretInfo(line, pos2);
		else if( token == "common" )
			InterpretCommon(line, pos2);
		else if( token == "char" )
			InterpretChar(line, pos2);
		else if( token == "kerning" )
			InterpretKerning(line, pos2);
		else if( token == "page" )
			InterpretPage(line, pos2, fontFile);
	}
	
	fclose(f);
	
	// Success
	return 0;
}

int CFontLoaderTextFormat::SkipWhiteSpace(string &str, int start)
{
	UINT n = start;
	while( n < str.size() )
	{
		char ch = str[n];
		if( ch != ' ' && 
		   ch != '\t' && 
		   ch != '\r' && 
		   ch != '\n' )
			break;
		
		++n;
	}
	
	return n;
}

int CFontLoaderTextFormat::FindEndOfToken(string &str, int start)
{
	UINT n = start;
	if( str[n] == '"' )
	{
		n++;
		while( n < str.size() )
		{
			char ch = str[n];
			if( ch == '"' )
			{
				// Include the last quote char in the token
				++n;
				break;
			}
			++n;
		}
	}
	else
	{
		while( n < str.size() )
		{
			char ch = str[n];
			if( ch == ' ' ||
			   ch == '\t' ||
			   ch == '\r' ||
			   ch == '\n' ||
			   ch == '=' )
				break;
			
			++n;
		}
	}
	
	return n;
}

void CFontLoaderTextFormat::InterpretKerning(string &str, int start)
{
	// Read the attributes
	int first = 0;
	int second = 0;
	int amount = 0;
	
	int pos, pos2 = start;
	while( true )
	{
		pos = SkipWhiteSpace(str, pos2);
		if( pos >= str.size() ) break;

		pos2 = FindEndOfToken(str, pos);
		
		string token = str.substr(pos, pos2-pos);
		
		pos = SkipWhiteSpace(str, pos2);
		if( pos >= str.size() || str[pos] != '=' ) break;
		
		pos = SkipWhiteSpace(str, pos+1);
		if( pos >= str.size() ) break;

		pos2 = FindEndOfToken(str, pos);
		
		string value = str.substr(pos, pos2-pos);
		
		if( token == "first" )
			first = strtol(value.c_str(), 0, 10);
		else if( token == "second" )
			second = strtol(value.c_str(), 0, 10);
		else if( token == "amount" )
			amount = strtol(value.c_str(), 0, 10);
		
	}
	
	// Store the attributes
	AddKerningPair(first, second, amount);
}

void CFontLoaderTextFormat::InterpretChar(string &str, int start)
{
	// Read all attributes
	int id = 0;
	int x = 0;
	int y = 0;
	int width = 0;
	int height = 0;
	int xoffset = 0;
	int yoffset = 0;
	int xadvance = 0;
	int page = 0;
	int chnl = 0;
	
	int pos, pos2 = start;
	while( true )
	{
		pos = SkipWhiteSpace(str, pos2);
		if( pos >= str.size() ) break;

		pos2 = FindEndOfToken(str, pos);
		
		string token = str.substr(pos, pos2-pos);
		
		pos = SkipWhiteSpace(str, pos2);
		if( pos >= str.size() || str[pos] != '=' ) break;
		
		pos = SkipWhiteSpace(str, pos+1);
		if( pos >= str.size() ) break;

		pos2 = FindEndOfToken(str, pos);
		
		string value = str.substr(pos, pos2-pos);
		
		if( token == "id" )
			id = strtol(value.c_str(), 0, 10);
		else if( token == "x" )
			x = strtol(value.c_str(), 0, 10);
		else if( token == "y" )
			y = strtol(value.c_str(), 0, 10);
		else if( token == "width" )
			width = strtol(value.c_str(), 0, 10);
		else if( token == "height" )
			height = strtol(value.c_str(), 0, 10);
		else if( token == "xoffset" )
			xoffset = strtol(value.c_str(), 0, 10);
		else if( token == "yoffset" )
			yoffset = strtol(value.c_str(), 0, 10);
		else if( token == "xadvance" )
			xadvance = strtol(value.c_str(), 0, 10);
		else if( token == "page" )
			page = strtol(value.c_str(), 0, 10);
		else if( token == "chnl" )
			chnl = strtol(value.c_str(), 0, 10);
		
	}
	
	// Store the attributes
	AddChar(id, x, y, width, height, xoffset, yoffset, xadvance, page, chnl);
}

void CFontLoaderTextFormat::InterpretCommon(string &str, int start)
{
	int fontHeight;
	int base;
	int scaleW;
	int scaleH;
	int pages;
	int packed;
	
	// Read all attributes
	int pos, pos2 = start;
	while( true )
	{
		pos = SkipWhiteSpace(str, pos2);
		if( pos >= str.size() ) break;

		pos2 = FindEndOfToken(str, pos);
		
		string token = str.substr(pos, pos2-pos);
		
		pos = SkipWhiteSpace(str, pos2);
		if( pos >= str.size() || str[pos] != '=' ) break;
		
		pos = SkipWhiteSpace(str, pos+1);
		if( pos >= str.size() ) break;

		pos2 = FindEndOfToken(str, pos);
		
		string value = str.substr(pos, pos2-pos);
		
		if( token == "lineHeight" )
			fontHeight = (short)strtol(value.c_str(), 0, 10);
		else if( token == "base" )
			base = (short)strtol(value.c_str(), 0, 10);
		else if( token == "scaleW" )
			scaleW = (short)strtol(value.c_str(), 0, 10);
		else if( token == "scaleH" )
			scaleH = (short)strtol(value.c_str(), 0, 10);
		else if( token == "pages" )
			pages = strtol(value.c_str(), 0, 10);
		else if( token == "packed" )
			packed = strtol(value.c_str(), 0, 10);
	}
	
	SetCommonInfo(fontHeight, base, scaleW, scaleH, pages, packed ? true : false);
}

void CFontLoaderTextFormat::InterpretInfo(string &str, int start)
{
	int outlineThickness;
	
	// Read all attributes
	int pos, pos2 = start;
	while( true )
	{
		pos = SkipWhiteSpace(str, pos2);
		if( pos >= str.size() ) break;

		pos2 = FindEndOfToken(str, pos);
		
		string token = str.substr(pos, pos2-pos);
		
		pos = SkipWhiteSpace(str, pos2);
		if( pos >= str.size() || str[pos] != '=' ) break;
		
		pos = SkipWhiteSpace(str, pos+1);
		if( pos >= str.size() ) break;

		pos2 = FindEndOfToken(str, pos);
		
		string value = str.substr(pos, pos2-pos);
		
		if( token == "outline" )
			outlineThickness = (short)strtol(value.c_str(), 0, 10);
		
	}
	
	SetFontInfo(outlineThickness);
}

void CFontLoaderTextFormat::InterpretPage(string &str, int start, const char *fontFile)
{
	int id = 0;
	string file;
	
	// Read all attributes
	int pos, pos2 = start;
	while( true )
	{
		pos = SkipWhiteSpace(str, pos2);
		if( pos >= str.size() ) break;

		pos2 = FindEndOfToken(str, pos);
		
		string token = str.substr(pos, pos2-pos);
		
		pos = SkipWhiteSpace(str, pos2);
		if( pos >= str.size() || str[pos] != '=' ) break;
		
		pos = SkipWhiteSpace(str, pos+1);
		if( pos >= str.size() ) break;

		pos2 = FindEndOfToken(str, pos);
		
		string value = str.substr(pos, pos2-pos);
		
		if( token == "id" )
			id = strtol(value.c_str(), 0, 10);
		else if( token == "file" )
			file = value.substr(1, value.length()-2);
		
	}
	
	LoadPage(id, file.c_str(), fontFile);
}

#if 0

//=============================================================================
// CFontLoaderBinaryFormat
//
// This class implements the logic for loading a BMFont file in binary format
//=============================================================================

CFontLoaderBinaryFormat::CFontLoaderBinaryFormat(FILE *f, CFont *font, const char *fontFile) : CFontLoader(f, font, fontFile)
{
}

int CFontLoaderBinaryFormat::Load()
{
	// Read and validate the tag. It should be 66, 77, 70, 2, 
	// or 'BMF' and 2 where the number is the file version.
	char magicString[4];
	fread(magicString, 4, 1, f);
	if( strncmp(magicString, "BMF\003", 4) != 0 )
	{
		RUDE_REPORT("Unrecognized format for '%s'", fontFile);
		fclose(f);
		return -1;
	}
	
	// Read each block
	char blockType;
	int blockSize;
	while( fread(&blockType, 1, 1, f) )
	{
		// Read the blockSize
		fread(&blockSize, 4, 1, f);
		
		switch( blockType )
		{
			case 1: // info
				ReadInfoBlock(blockSize);
				break;
			case 2: // common
				ReadCommonBlock(blockSize);
				break;
			case 3: // pages
				ReadPagesBlock(blockSize);
				break;
			case 4: // chars
				ReadCharsBlock(blockSize);
				break;
			case 5: // kerning pairs
				ReadKerningPairsBlock(blockSize);
				break;
			default:
				RUDE_REPORT("Unexpected block type (%d)", blockType);
				fclose(f);
				return -1;
		}
	}
	
	fclose(f);
	
	// Success
	return 0;
}

void CFontLoaderBinaryFormat::ReadInfoBlock(int size)
{
//#pragma pack(push)
//#pragma pack(1)
	struct infoBlock
	{
		WORD fontSize;
		BYTE reserved:4;
		BYTE bold    :1;
		BYTE italic  :1;
		BYTE unicode :1;
		BYTE smooth  :1;
		BYTE charSet;
		WORD stretchH;
		BYTE aa;
		BYTE paddingUp;
		BYTE paddingRight;
		BYTE paddingDown;
		BYTE paddingLeft;
		BYTE spacingHoriz;
		BYTE spacingVert;
		BYTE outline;         // Added with version 2
		char fontName[1];
	};
//#pragma pack(pop)
	
	char *buffer = new char[size];
	fread(buffer, size, 1, f);
	
	// We're only interested in the outline thickness
	infoBlock *blk = (infoBlock*)buffer;
	SetFontInfo(blk->outline);
	
	delete[] buffer;
}

void CFontLoaderBinaryFormat::ReadCommonBlock(int size)
{
//#pragma pack(push)
//#pragma pack(1)
	struct commonBlock
	{
		WORD lineHeight;
		WORD base;
		WORD scaleW;
		WORD scaleH;
		WORD pages;
		BYTE packed  :1;
		BYTE reserved:7;
		BYTE alphaChnl;
		BYTE redChnl;
		BYTE greenChnl;
		BYTE blueChnl;
	}; 
//#pragma pack(pop)
	
	char *buffer = new char[size];
	fread(buffer, size, 1, f);
	
	commonBlock *blk = (commonBlock*)buffer;
	
	SetCommonInfo(blk->lineHeight, blk->base, blk->scaleW, blk->scaleH, blk->pages, blk->packed ? true : false);
	
	delete[] buffer;
}

void CFontLoaderBinaryFormat::ReadPagesBlock(int size)
{
//#pragma pack(push)
//#pragma pack(1)
	struct pagesBlock
	{
		char pageNames[1];
	};
//#pragma pack(pop)
	
	char *buffer = new char[size];
	fread(buffer, size, 1, f);
	
	pagesBlock *blk = (pagesBlock*)buffer;
	
	for( int id = 0, pos = 0; pos < size; id++ )
	{
		LoadPage(id, &blk->pageNames[pos], fontFile);
		pos += 1 + (int)strlen(&blk->pageNames[pos]);
	}
	
	delete[] buffer;
}

void CFontLoaderBinaryFormat::ReadCharsBlock(int size)
{
//#pragma pack(push)
//#pragma pack(1)
	struct charsBlock
	{
		struct charInfo
		{
			DWORD id;
			WORD  x;
			WORD  y;
			WORD  width;
			WORD  height;
			short xoffset;
			short yoffset;
			short xadvance;
			BYTE  page;
			BYTE  chnl;
		} chars[1];
	};
//#pragma pack(pop)
	
	char *buffer = new char[size];
	fread(buffer, size, 1, f);
	
	charsBlock *blk = (charsBlock*)buffer;
	
	for( int n = 0; int(n*sizeof(charsBlock::charInfo)) < size; n++ )
	{
		AddChar(blk->chars[n].id,
				blk->chars[n].x,
				blk->chars[n].y,
				blk->chars[n].width,
				blk->chars[n].height,
				blk->chars[n].xoffset,
				blk->chars[n].yoffset,
				blk->chars[n].xadvance,
				blk->chars[n].page,
				blk->chars[n].chnl);
	}
	
	delete[] buffer;
}

void CFontLoaderBinaryFormat::ReadKerningPairsBlock(int size)
{
//#pragma pack(push)
//#pragma pack(1)
	struct kerningPairsBlock
	{
		struct kerningPair
		{
			DWORD first;
			DWORD second;
			short amount;
		} kerningPairs[1];
	};
//#pragma pack(pop)
	
	char *buffer = new char[size];
	fread(buffer, size, 1, f);
	
	kerningPairsBlock *blk = (kerningPairsBlock*)buffer;
	
	for( int n = 0; int(n*sizeof(kerningPairsBlock::kerningPair)) < size; n++ )
	{
		AddKerningPair(blk->kerningPairs[n].first,
					   blk->kerningPairs[n].second,
					   blk->kerningPairs[n].amount);
	}
	
	delete[] buffer;
}
	
#endif

// 2008-05-11 Storing the characters in a map instead of an array
// 2008-05-11 Loading the new binary format for BMFont v1.10
// 2008-05-17 Added support for writing text with UTF8 and UTF16 encoding
