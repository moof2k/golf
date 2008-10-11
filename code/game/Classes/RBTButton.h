/*
 *  RBTButton.h
 *  RudeBlocks
 *
 *  Created by Robert Rose on 8/27/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __H_RBTButton
#define __H_RBTButton

#include "RBTouchTracker.h"

class RBTButton {

public:
	RBTButton();
	~RBTButton();
	
	void Render(Rude3DSurface *backbuffer3d);
	
	bool Hit(RudeScreenVertex &p);
	
	void SetTexture(const char *name);
	void SetPosition(float x, float y)
	{
		_x = x;
		_y = y;
	}
	void SetFlipHorz(bool b) { _fliphorz = b; }
	void SetSize(float w, float h) { _w = w; _h = h; }
	
private:
	
	RBTouch *_touch;
	float _x, _y;
	float _w, _h;
	int _tid;
	bool _fliphorz;
	
};

#endif
