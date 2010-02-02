#ifndef STRICT
#define STRICT
#endif

#include "RBTTitle.h"
#include "RudeMath.h"

RBTTitle::RBTTitle()
{

	_keymap = RBKeyMap::getInstance();

	_titlerude = NULL;
	_titlebg = NULL;

	Init();
}

RBTTitle::~RBTTitle()
{
	Destroy();
}

void RBTTitle::Destroy()
{

}

void RBTTitle::Init()
{
	Destroy();

	_result = TitleStartGame;
	_done = false;

	_ltimer = 0.0f;
}


void RBTTitle::CreateSurfaces(RudeSurface *reference)
{
	if(_titlerude == NULL)
		_titlerude = RudeSurface::CreateSurface(reference, _T("title_rude.bmp"));
	if(_titlebg == NULL)
		_titlebg = RudeSurface::CreateSurface(reference, _T("title_bg.bmp"));
}

void RBTTitle::Render(RudeSurface &screen, float delta, float aspect)
{
	_ltimer += delta;

	//screen.BltFast(0, 0, _titlebg, NULL);

	RudeRect dstrect;
	dstrect.top = 0;
	dstrect.left = 0;
	dstrect.bottom = _titlerude->GetHeight() - 1;
	dstrect.right = _titlerude->GetWidth() - 1;

	int halfheight = _titlerude->GetHeight() / 2;
	int halfwidth = _titlerude->GetWidth() / 2;
	
	int halfsheight = screen.GetHeight() / 2;
	int halfswidth = screen.GetWidth() / 2;

	int oy = -31;
	int ox = -1;

	float t1 = 1.0f;
	float t2 = 2.0f;
	float t3 = 0.7f;
	float ts = 3.0f;
	float tm = 1.0f;

	if(_ltimer * ts < t1)
	{
		tm = _ltimer * ts;
	}
	else if(_ltimer > t3)
	{
		tm = 1.0f;
	}
	else
	{
		t1 = 1.0f;
		tm = (0.3f * sin(((_ltimer * ts) - t1) * 20.0f)) / (_ltimer * 15.0f) + t1;
	}

	dstrect.top = halfsheight - halfheight * tm + oy;
	dstrect.left = halfswidth - halfwidth * tm + ox;
	dstrect.bottom = halfsheight + halfheight * tm + oy;
	dstrect.right = halfswidth + halfwidth * tm + ox;
	
	//screen.Blt(&dstrect, _titlerude, NULL, true);

}


void RBTTitle::KeyDown(RudeKey k)
{

}

void RBTTitle::KeyUp(RudeKey k)
{
	if(k == kRudeKeyUp)
	{
		_done = true;
		_result = TitleStartGame;
	}
	else
	{
		Init();
	}
}

void RBTTitle::StylusDown(RudeScreenVertex &p)
{
	
}

void RBTTitle::StylusUp(RudeScreenVertex &p)
{
	
}

void RBTTitle::StylusMove(RudeScreenVertex &p)
{
	
}

void RBTTitle::Pause()
{

}


