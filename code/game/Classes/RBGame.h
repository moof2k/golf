/*	RBGame.h
 *	
 *	Copyright (c) 2003-2005 Robert W. Rose
 *	
 *	ALL RIGHTS RESERVED
 *	Reproduction, adaptation, or translation without prior
 *	written permission is prohibited, except as allowed
 *	under copyright law.
 *	
 *	RESTRICTED RIGHTS LEGEND
 *	Use, duplication or disclosure by the Goverment is
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

#ifndef _H_RBGame
#define _H_RBGame


#include "RudeGame.h"
#include "RudeTouchTracker.h"
#include "RBKeyMap.h"

#include "RBTRound.h"


typedef enum {
	GameIntroLogo = 1,
	GameTitle,
	GameRBT,
	
} GameState;



#define kRBTMaxPieces 256
#define kRBTHeight 23
#define kRBTWidth 10


class RBGame : public RudeGame
{
// Construction
public:
	RBGame();
	~RBGame();


	void Init();
	void Destroy();

	void Render(float delta, float aspect);
	
	void KeyUp(RudeKey k);
	void KeyDown(RudeKey k);
	void StylusDown(RudeScreenVertex &p);
	void StylusMove(RudeScreenVertex &p);
	void StylusUp(RudeScreenVertex &p);
	
	void TouchDown(RudeScreenVertex &n);
	void TouchMove(RudeScreenVertex &n, RudeScreenVertex &p);
	void TouchUp(RudeScreenVertex &n, RudeScreenVertex &p);

	void Pause();

private:

	GameState	_state;

	RBTRound	*_rbt;

	RBKeyMap	*_keymap;
	
	RudeTouchTracker _touchtracker;

};

#endif 
