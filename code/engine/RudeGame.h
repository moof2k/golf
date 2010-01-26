/*
 *  RudeGame.h
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef __H_RudeGame
#define __H_RudeGame

#include "RudeGlobals.h"
#include "RudeDebug.h"
#include "RudeKey.h"
#include "RudeVertex.h"

#define REC_FRAMES 10

#ifdef RUDE_SYMBIAN
#include <e32std.h>
#endif

/**
 * Base class for games made using the engine.  You're not required
 * to use this base class, but it provides some useful convenience functions.
 */
class RudeGame {

public:

	RudeGame();
	virtual ~RudeGame();

	
	void Render();
	virtual void NextFrame(float delta);
	virtual void Render(float aspect);
	
	virtual void KeyDown(RudeKey k);
	virtual void KeyUp(RudeKey k);
	virtual void StylusDown(RudeScreenVertex &p);
	virtual void StylusMove(RudeScreenVertex &p);
	virtual void StylusUp(RudeScreenVertex &p);
	
	virtual void TouchDown(RudeScreenVertex &n);
	virtual void TouchMove(RudeScreenVertex &n, RudeScreenVertex &p);
	virtual void TouchUp(RudeScreenVertex &n, RudeScreenVertex &p);

	virtual void Pause();
	virtual bool Done() { return m_done; }

	static float GetTime();
	
protected:
	bool m_done;

private:
	float _rendertimes[REC_FRAMES];
	int _rti;

	float GetDelta();
	
	float _rotate;


#ifdef RUDE_PALMOS	
	UInt32 _prevtime;
#endif

#ifdef RUDE_SYMBIAN
	TTime _prevtime;
#endif

#ifdef RUDE_WIN
	float _prevtime;
#endif
};

#endif
