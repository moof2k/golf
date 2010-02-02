/*	RBTTitle.h
 *	
 *	Copyright (c) 2003-2005 Robert W. Rose
 *	
 *	ALL RIGHTS RESERVED
 *	Reproduction, adaptation, or translation without prior
 *	written permission is prohibited, except as allowed
 *	under copyright law.
 *	
 *	RESTRICTED RIGHTS LEGEND
 *	Use, duplication or disclosure by the Government is
 *	subject to restrictions as set for in subdivision
 *	(b)(3)(ii) of the Right in Technical Data and Computer
 *	Software clause at 52.227-7013.
 *	
 *	Robert W. Rose
 *	421 SW 10th St
 *	Corvallis, OR 97333
 *	
 *	robertwrose.com
 */

#ifndef _H_RBTTitle
#define _H_RBTTitle


#include "RudeGame.h"

#include "RBKeyMap.h"

typedef enum {
	TitleStartGame = 1,
	TitleQuit
	
} TitleResult;


class RBTTitle : public RudeGame
{
// Construction
public:
	RBTTitle();
	~RBTTitle();


	void Init();
	void Destroy();

	bool Done() { return _done; }
	TitleResult Result() { return _result; }

	void CreateSurfaces(RudeSurface *reference);
	void Render(RudeSurface &screen, float delta, float aspect);
	
	void KeyUp(RudeKey k);
	void KeyDown(RudeKey k);
	void StylusDown(RudeScreenVertex &p);
	void StylusMove(RudeScreenVertex &p);
	void StylusUp(RudeScreenVertex &p);

	void Pause();

private:

	bool _done;
	TitleResult _result;

	RudeSurface *_titlebg, *_titlerude;

	RBKeyMap	*_keymap;

	float _ltimer;
};

#endif 
