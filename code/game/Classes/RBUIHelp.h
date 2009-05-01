/*
 *  RBUIHelp.h
 *  golf
 *
 *  Created by Robert Rose on 4/30/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __H_RBUIHelp
#define __H_RBUIHelp

#include "Rude.h"
#include "RudeButtonControl.h"


typedef enum {
	kHelpAim,
	kHelpPlacementCamera,
	kHelpSwing,
	
	kNumHelpModes,
} eHelpMode;

class RBUIHelp
{
public:
	
	RBUIHelp();
	~RBUIHelp();
	
	void SetHelpMode(eHelpMode mode);
	eHelpMode GetHelpMode() { return m_mode; }
	
	void NextFrame(float delta);
	void Render(float aspect);
	
	bool TouchDown();
	
	void Enable(bool enable) { m_enabled = enable; }
	bool Enabled() { return m_enabled; }

private:
	
	void NextHelp();
	
	void LoadTexture(const char *texname);
	
	float m_bgTimer;
	int m_helpTexture;
	
	eHelpMode m_mode;
	int m_helpNumber;
	
	bool m_done;
	bool m_enabled;
	
};

#endif
