/*
 *  RudeGame.h
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009-2011 Bork 3D LLC. All rights reserved.
 *
 *	Permission is granted to use this software, in source code or binary form,
 *	with or without modification, for NONCOMMERCIAL PURPOSES provided:
 *	
 *	1) The user of this software (YOU) does not do so in a means intended to
 *	derive monetary compensation or commercial advantage.
 *	
 *	2) Redistributions of the source code contain this license notice, unmodified.
 *	
 *	3) Redistributions in binary form give credit to this software using the
 *	text, "Made with the Bork 3D Game Engine," either in an "About," "Credits,"
 *	or other prominent location in the binary run-time form.
 *	
 *	Commercial users of this software must obtain a Bork 3D Game Engine Commercial
 *	Use License.  See http://bork3d.com for details.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *	THE SOFTWARE.
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
    
    virtual void ScrollWheel(RudeScreenVertex &d);

	virtual void Resize();

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
