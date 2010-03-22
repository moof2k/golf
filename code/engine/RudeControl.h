/*
 *  RudeControl.h
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef __H_RudeControl
#define __H_RudeControl

#include "RudeGL.h"
#include "RudeRect.h"
#include "RudeTouchTracker.h"

#include <vector>
#include <string>
#include <list>

typedef enum {
	kAnimNone,
	kAnimConstant,
	kAnimPopSlide
} eAnimType;

class RudeTextControl;
class RudeButtonControl;

class RudeControl 
{
public:
	RudeControl();

	void Load(const char *name);
	void SetName(const std::string &name) { m_name = name; }
	const std::string & GetName() { return m_name; }

	RudeControl * GetChildControl(const std::string &name);
	RudeTextControl * GetChildTextControl(const std::string &name);
	RudeButtonControl * GetChildButtonControl(const std::string &name);
	
	virtual void SetRect(const RudeRect &r) { m_rect = r; }
	
	virtual bool Contains(const RudeScreenVertex &p);
	
	virtual bool TouchDown(RudeTouch *t);
	virtual bool TouchMove(RudeTouch *t);
	virtual bool TouchUp(RudeTouch *t);
	
	virtual void NextFrame(float delta);
	virtual void Render();
	
	RudeScreenVertex GetDistanceTraveled() { return m_hitDistanceTraveled; }
	RudeScreenVertex GetMoveDelta() { return m_hitMoveDelta; }
	RudeScreenVertex GetHitMove() { return m_hitMove; }
	
	void SetTranslation(const btVector3 &t) { m_translation = t; }
	void SetDesiredTranslation(const btVector3 &desired) { m_desiredTranslation = desired; }
	
	void SetAnimSpeed(float f) { m_animSpeed = f; }
	void SetAnimType(eAnimType at) { m_animType = at; }

	static std::string PopToken(std::list<std::string> &tokens, std::string &desc, const std::string &explanation);
	
protected:

	void ConstructChild(char *desc);
	void ParseRect(std::string &str, RudeRect &rect);
	void ParseColor(std::string &str, unsigned int &color);

	std::vector<RudeControl *> m_children;

	std::string m_name;
	
	RudeRect m_rect;
	RudeScreenVertex m_hitStart;
	RudeScreenVertex m_hitMove;
	RudeScreenVertex m_hitMoveDelta;
	RudeScreenVertex m_hitDelta;
	RudeScreenVertex m_hitDistanceTraveled;
	int m_hitId;
	
	btVector3 m_translation;
	btVector3 m_desiredTranslation;
	float m_animSpeed;
	eAnimType m_animType;
};

#endif
