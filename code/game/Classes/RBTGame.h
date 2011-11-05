/*
 *  RBTGame.h
 *  golf
 *
 *  Created by Robert Rose on 10/10/08.
 *  Copyright 2008 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef _H_RBTGame
#define _H_RBTGame

#include "RBGameBase.h"
#include "RudeObject.h"
#include "RudeSkybox.h"
#include "RudeTextControl.h"
#include "RudeButtonAnimControl.h"
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
#include "RBGolfPin.h"
#include "RBSwingControl.h"
#include "RBWindControl.h"
#include "RBTerrainSliceControl.h"
#include "RBBallGuide.h"
#include "RBGolferObject.h"
#include "RBGolfClub.h"
#include "RBBallRecorder.h"
#include "RBScoreControl.h"
#include "RBScoreTracker.h"
#include "RBCourseData.h"
#include "RBUIHelp.h"
#include "RBUIMenu.h"
#include "RBUITerrain.h"

typedef enum {
	kStateTeePosition,
	kStatePositionSwing,
	kStatePositionSwing2,
	kStatePositionSwing3,
	kStateMenu,
	kStateExecuteSwing,
	kStateWaitForSwing,
	kStateHitBall,
	kStateFollowBall,
	kStateRegardBall,
	kStateBallOutOfBounds,
	kStateBallInHole,
	kStateTerrainView
} eRBTGameState;

typedef enum {
	kResultNone,
	kResultComplete,
	kResultQuit
} eGameResult;

typedef struct {
	int size;
	
	eRBTGameState state;
	
	int curClub;
	
	float windDir;
	btVector3 windVec;
	float windSpeed;
	
	float swingPower;
	float swingAngle;
	
	float ballToHoleDist;
	
	btVector3 ball;
	eRBTerrainMaterial ballMaterial;
	
	btVector3 hole;
	
} tRBTGameStateSave;

class RBTGame : public RBGameBase
{
// Construction
public:
	RBTGame(int holeNum, const char *terrainfile, eCourseTee tee, eCourseHoles holeset, eCourseWind wind, int par, int numPlayers, bool restorestate);
	~RBTGame();

	void NextFrame(float delta);
	void Render(float width, float height);
	
	void LaunchBall();
	
	void KeyUp(RBKey k);
	void KeyDown(RBKey k);
	void StylusDown(RudeScreenVertex &p);
	void StylusMove(RudeScreenVertex &p);
	void StylusUp(RudeScreenVertex &p);
	
	void TouchDown(RudeTouch *rbt);
	void TouchMove(RudeTouch *rbt);
	void TouchUp(RudeTouch *rbt);
    
    void ScrollWheel(RudeScreenVertex &d);

	void Resize();

	void Pause();
	
	eGameResult GetResult() { return m_result; }
	
	void SaveState();
	int LoadState();

private:
	
	void SetupUI();
	
	void RestoreState();
	
	void SetState(eRBTGameState state);
	
	void StateTeePosition(float delta);
	void StatePositionSwing(float delta);
	void StatePositionSwing2(float delta);
	void StatePositionSwing3(float delta);
	void StateHitBall(float delta);
	void StateFollowBall(float delta);
	void StateRegardBall(float delta);
	
	void RenderCalcOrthoDrawPositions();
	void RenderGuide(float aspect);
	void RenderBallFollowInfo(bool showDistToHole);
	void RenderShotInfo(bool showShotDistance, bool showClubInfo);
	
	void FreshShotEncouragement();
	void AutoSelectClub();
	void NextClub(int n);
	void MovePosition(const RudeScreenVertex &p, const RudeScreenVertex &dist);
    void MoveSwingYaw(float dx);
    void MoveSwingHeight(float dy);
	void MoveAimCamera(const RudeScreenVertex &p, const RudeScreenVertex &dist);
    void MoveAimYaw(float dx);
    void MoveAimHeight(float dy);
	void FreshGuide(bool firstTime = false);
	void StickBallInBounds();
	void HitBall();
	void AdjustGuide();
	void SetHoleHeightText();
	
	
	RBUIMenu m_menu;
	RBUIHelp m_help;
	RBUITerrain m_terrainui;
	
	eRBTGameState m_state;
	eGameResult m_result;
	
	RBTerrain m_terrain;
	RBGolfBall m_ball;
	RBGolfPin m_pin;
	RudeSkybox m_skybox;
	RBGolferObject m_golfer;
	RBBallRecorder m_ballRecorder;
	
	RBDebugCamera m_debugCamera;
	RBBallCamera m_ballCamera;
	RBCamera *m_curCamera;
	
	
	RBBallGuide m_ballGuide;
	RBSwingControl *m_swingControl;
	RBWindControl *m_windControl;
	RBTerrainSliceControl * m_terrainSliceControl;
	RudeButtonAnimControl *m_swingButton;
	RudeButtonControl *m_moveButton;
	RudeButtonControl *m_menuButton;
	RudeControl *m_swingCamAdjust;
	RudeControl *m_guideAdjust;
	float m_swingYaw;
	float m_swingHeight;
	float m_swingCamYaw;
	bool m_moveGuide;
	bool m_moveHeight;
	
	int m_curClub;

	RudeControl m_ui;

	RudeButtonControl *m_botBarBg;
	RudeButtonControl *m_nextClubButton;
	RudeButtonControl *m_prevClubButton;
	RudeButtonControl *m_clubButton;
	RudeButtonControl *m_cameraButton;
	RudeButtonControl *m_helpButton;
	RudeButtonControl *m_terrainButton;
	RudeButtonControl m_guideIndicatorButton;
	RudeButtonControl m_holeIndicatorButton;
	
	RudeTextControl *m_holeText;
	RudeTextControl *m_parText;
	RudeTextControl *m_strokeText;
	RudeTextControl *m_remainingDistText;
	RudeTextControl *m_scoreText;
	RudeTextControl *m_shotEncouragementText;
	RudeTextControl *m_clubDistText;
	RudeTextControl *m_powerRangeText;
	RudeTextControl *m_windText;
	
	RudeTextControl *m_shotDistText;
	RudeTextControl *m_shotPowerText;
	RudeTextControl *m_shotAngleText;
	
	RudeTextControl *m_guidePowerText;

	RudeTextControl m_holeHeightText;
	
	RBScoreControl *m_scoreControl;

	bool m_guideScreenCalc;
	RudeScreenVertex m_guideScreenPoint;
	
	float m_swingPower;
	float m_swingAngle;
	
	float m_followTimer;
	float m_stopTimer;
	float m_encouragementTimer;
	float m_oobTimer;
	
	float m_windDir;
	float m_windSpeed;
	btVector3 m_windVec;
	
	float m_placementGuidePower;
	
	btVector3 m_dBall;
	btVector3 m_guidePosition;
	btVector3 m_placementGuidePosition;
	btVector3 m_guidePositionScreenSpace;
	btVector3 m_holePositionScreenSpace;
	btVector3 m_placementGuidePositionScreenSpace;
	
	btVector3 m_ballLastInBoundsPosition;
	
	float m_ballShotDist;
	float m_ballToHoleDist;
	
	bool m_playedBallDissapointmentSound;
	
	bool m_landscape;
	
	int m_holeNum;
	int m_par;
	int m_curPlayer;
	int m_numPlayers;
	
	eCourseHoles m_holeSet;
	eCourseTee m_tee;
	eCourseWind m_wind;
	
	RudeTextControl m_dropDecoText;
	RudeTextControl m_dumpDecoText;
};

#endif 
