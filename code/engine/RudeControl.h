/*
 *  RudeControl.h
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

#ifndef __H_RudeControl
#define __H_RudeControl

#include "RudeGL.h"
#include "RudeRect.h"
#include "RudeTouchTracker.h"

#include <vector>
#include <string>
#include <list>
#include <map>

typedef enum {
	kAnimNone,
	kAnimConstant,
	kAnimPopSlide
} eAnimType;

class RudeControl;
class RudeTextControl;
class RudeButtonControl;
class RudeButtonAnimControl;

typedef RudeControl * (*ConstructRudeControlFuncPtr)(RudeControl *, std::list<std::string> &, const std::string &);

class RudeControl 
{
public:
	RudeControl(RudeControl *parent);
	virtual ~RudeControl();

	void Load(const char *name);
	void SetName(const std::string &name) { m_name = name; }
	const std::string & GetName() { return m_name; }

	RudeControl * GetChildControl(const std::string &name);

	void SetParent(RudeControl *parent) { m_parent = parent; }
	RudeControl * GetParent() { return m_parent; }

	template <typename T>
	T * GetChildControl(const std::string &name)
	{
		T *child = dynamic_cast<T *>(GetChildControl(name));

		RUDE_ASSERT(child, "Child control '%s' not of expected type", name.c_str());

		return child;
	}

	void SetFileRect(const RudeRect &r);
	RudeRect GetFileRect() { return m_fileRect; }

	void SetDrawRect(const RudeRect &r);
	RudeRect GetDrawRect() { return m_drawRect; }

	void UpdateDrawRect();
	virtual void OnReposition() {}
	
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
    
    void SetAdjustDrawRectToEdges(bool a) { m_adjustDrawRectToEdges = a; }

	static std::string PopToken(std::list<std::string> &tokens, const std::string &desc, const std::string &explanation);
	static void ParseRect(std::string &str, RudeRect &rect);
	static void ParseOffset(std::string &str, int &offx, int &offy);
	static void ParseColor(std::string &str, unsigned int &color);
	
protected:

	void ConstructChild(char *desc);

	std::vector<RudeControl *> m_children;
	RudeControl *m_parent;

	std::string m_name;
	
	RudeRect m_fileRect;
	RudeRect m_drawRect;

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

    /**
     * If true, negative values in m_fileRect are interpreted as distances
     * from their respective edges.  I.e., right = -1 means 0 pixels from
     * the right edge of the parent control.
     */
    bool m_adjustDrawRectToEdges;
};


class RudeControlRegistration
{
public:
	typedef std::map<std::string, ConstructRudeControlFuncPtr> tConstructorList;

	RudeControlRegistration(const std::string &name, ConstructRudeControlFuncPtr regfunc)
	{
		tConstructorList &list = GetConstructorList();

		std::map<std::string, ConstructRudeControlFuncPtr>::iterator constructor = list.find(name);

		RUDE_ASSERT(constructor == list.end(), "Two duplicate constructors for %s are being registered.. not OK", name.c_str());

		list[name] = regfunc;
	}

	static ConstructRudeControlFuncPtr GetConstructor(const std::string &name)
	{
		tConstructorList &list = GetConstructorList();

		std::map<std::string, ConstructRudeControlFuncPtr>::iterator constructor = list.find(name);

		RUDE_ASSERT(constructor != list.end(), "Constructor for type %s not found", name.c_str());

		return constructor->second;
	}

private:

	static tConstructorList & GetConstructorList()
	{
		static tConstructorList *list = 0;

		if(list == 0)
		{
			list = new tConstructorList();
		}

		return *list;
	}
};

#endif
