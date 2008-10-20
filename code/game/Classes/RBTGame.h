/*	RBTGame.h
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

#ifndef _H_RBTGame
#define _H_RBTGame


#include "RudeGame.h"
#include "RudeObject.h"
#include "RudeSkybox.h"
#include "RudeTextControl.h"
#include "RudeButtonControl.h"
#include "RBKeyTracker.h"
#include "RudeFont.h"
#include "RudeTouchTracker.h"
#include "RBKeyMap.h"
#include "RBCamera.h"
#include "RBDebugCamera.h"
#include "RBBallCamera.h"
#include "RBTerrain.h"
#include "RBGolfBall.h"
#include "RBSwingControl.h"
#include "RBBallGuide.h"
#include "RBGolferObject.h"
#include "RBGolfClub.h"
#include "RBBallRecorder.h"
#include "RBScoreControl.h"
#include "RBScoreTracker.h"


typedef enum {
	kStateTeePosition,
	kStatePositionSwing,
	kStatePositionSwing2,
	kStateExecuteSwing,
	kStateWaitForSwing,
	kStateHitBall,
	kStateFollowBall,
	kStateRegardBall,
	kStateBallOutOfBounds,
	kStateBallInHole
} eRBTGameState;

class RBTGame : public RudeGame
{
// Construction
public:
	RBTGame(int holeNum, const char *terrainfile, int par, int numPlayers);
	~RBTGame();

	void NextFrame(float delta);
	void Render(float aspect);
	
	void LaunchBall();
	
	void KeyUp(RBKey k);
	void KeyDown(RBKey k);
	void StylusDown(RudeScreenVertex &p);
	void StylusMove(RudeScreenVertex &p);
	void StylusUp(RudeScreenVertex &p);
	
	void TouchDown(RudeTouch *rbt);
	void TouchMove(RudeTouch *rbt);
	void TouchUp(RudeTouch *rbt);

	void Pause();
	

private:
	
	void SetState(eRBTGameState state);
	
	void StateTeePosition(float delta);
	void StatePositionSwing(float delta);
	void StatePositionSwing2(float delta);
	void StateHitBall(float delta);
	void StateFollowBall(float delta);
	
	void RenderCalcOrthoDrawPositions();
	void RenderGuide(float aspect);
	void RenderBallFollowInfo(bool showDistToHole);
	void RenderShotInfo(bool showShotDistance, bool showClubInfo);
	
	void FreshShotEncouragement();
	void AutoSelectClub();
	void NextClub(int n);
	void MovePosition(const RudeScreenVertex &p, const RudeScreenVertex &dist);
	void MoveAimCamera(const RudeScreenVertex &p, const RudeScreenVertex &dist);
	void FreshGuide();
	void StickBallInBounds();
	void HitBall();
	
	
	eRBTGameState m_state;
	
	RBTerrain m_terrain;
	RBGolfBall m_ball;
	RudeSkybox m_skybox;
	RBGolferObject m_golfer;
	RBBallRecorder m_ballRecorder;
	
	RBDebugCamera m_debugCamera;
	RBBallCamera m_ballCamera;
	RBCamera *m_curCamera;
	
	
	RBBallGuide m_ballGuide;
	RBSwingControl m_swingControl;
	RudeButtonControl m_swingButton;
	RudeButtonControl m_moveButton;
	RudeControl m_swingCamAdjust;
	float m_swingYaw;
	float m_swingHeight;
	float m_swingCamYaw;
	bool m_moveGuide;
	bool m_moveHeight;
	
	int m_curClub;
	RudeButtonControl m_nextClubButton;
	RudeButtonControl m_prevClubButton;
	RudeButtonControl m_clubButton;
	RudeButtonControl m_cameraButton;
	RudeButtonControl m_guideIndicatorButton;
	
	RudeTextControl m_parText;
	RudeTextControl m_strokeText;
	RudeTextControl m_remainingDistText;
	RudeTextControl m_scoreText;
	RudeTextControl m_shotEncouragementText;
	RudeTextControl m_shotQualityText;
	
	RBScoreControl m_scoreControl;
	
	float m_swingPower;
	float m_swingAngle;
	
	float m_followTimer;
	float m_stopTimer;
	float m_encouragementTimer;
	
	btVector3 m_dBall;
	btVector3 m_guidePosition;
	btVector3 m_guidePositionScreenSpace;
	
	btVector3 m_ballLastInBoundsPosition;
	
	float m_ballShotDist;
	float m_ballToHoleDist;
	
	int m_holeNum;
	int m_par;
	int m_curPlayer;
	int m_numPlayers;
};

#endif 
