/*	RBKeyTracker.h
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

#ifndef _H_RPGKeyTracker
#define _H_RPGKeyTracker

#include "RBKeyMap.h"

#define kKTMaxKeys 5

class RBKeyTracker
{
public:
	RBKeyTracker(void);
	~RBKeyTracker(void);

	void init(void);

	void keyDown(RBKey k);
	void keyUp(RBKey k);

	bool isDown(RBKey k);

	void getKeyStr(TCHAR *str);

	bool timer(float delta);

private:

	float _keytimer;
	float _timermax;

	RBKey	_keysdown[kKTMaxKeys];

};

#endif
