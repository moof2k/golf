/*
 *  RBTGame.cpp
 *  golf
 *
 *  Created by Robert Rose on 10/10/08.
 *  Copyright 2008 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef STRICT
#define STRICT
#endif

#include "RBTGame.h"
#include "RudeGL.h"
#include "RudeGLD.h"
#include "RudeTextureManager.h"
#include "RudeText.h"
#include "RudeFile.h"
#include "RudeDebug.h"
#include "RudePerf.h"
#include "RudePhysics.h"
#include "RudeRegistry.h"
#include "RudeTimer.h"
#include "RudeTweaker.h"
#include "RudeSound.h"
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>

#include "btTransform.h"

bool gDebugCamera = false;
RUDE_TWEAK(DebugCamera, kBool, gDebugCamera);
bool gDebugCameraPrev = false;

bool gDebugFinishHole = false;
RUDE_TWEAK(DebugFinishHole, kBool, gDebugFinishHole);

const unsigned int kBallDistanceTopColor = 0xFF666666;
const unsigned int kBallDistanceBotColor = 0xFF000000;
const unsigned int kBallDistanceOutlineTopColor = 0xFF00FFFF;
const unsigned int kBallDistanceOutlineBotColor = 0xFF22FFFF;

const unsigned int kBallRemainingTopColor = 0xFF666666;
const unsigned int kBallRemainingBotColor = 0xFF000000;
const unsigned int kBallRemainingOutlineTopColor = 0xFFFFFFFF;
const unsigned int kBallRemainingOutlineBotColor = 0xFFFFFFFF;

const unsigned int kParTopColor = 0xFFF69729;
const unsigned int kParBotColor = 0xFF000000;
const unsigned int kParOutlineTopColor = 0xFFFFFFFF;
const unsigned int kParOutlineBotColor = 0xFFFFFFFF;

const float kFollowTimerThreshold = 2.0f;

RBTGame::RBTGame(int holeNum, const char *terrainfile, eCourseTee tee, eCourseHoles holeset, eCourseWind wind, int par, int numPlayers, bool restorestate)
{	
	m_result = kResultNone;
	
	RudePhysics::GetInstance()->Init();
		
	m_terrain.Load(terrainfile);
	m_terrain.SetTee(tee);
	
	m_holeNum = holeNum;
	m_par = par;
	m_numPlayers = numPlayers;
	m_tee = tee;
	m_holeSet = holeset;
	m_wind = wind;
	m_curPlayer = 0;
	
	m_playedBallDissapointmentSound = false;
	
	m_ball.Load("ball_1");
	m_ball.SetPosition(btVector3(0,100,0));
	m_ball.SetCurMaterial(kTee);
	
	m_ballRecorder.SetBall(&m_ball);
	
	m_ballGuide.SetBall(&m_ball);
	
	m_golfer.Load("golfer");
	
	m_skybox.Load("skybox");
	
	m_ballCamera.SetBall(&m_ball);
	m_ballCamera.SetTerrain(&m_terrain);
	
	if(gDebugCamera)
		m_curCamera = &m_debugCamera;
	else
		m_curCamera = &m_ballCamera;
	
	m_guideScreenCalc = false;
	
	// score control
	m_scoreControl.SetRect(RudeRect(0,0,480,320));
	m_scoreControl.SetActiveHole(m_holeNum, holeset);
	
	// stroke/status controls
	
	m_holeText.SetAlignment(kAlignLeft);
	m_holeText.SetPosition(10, 30);
	m_holeText.SetFormat(kIntValue, "%d");
	m_holeText.SetValue(m_holeNum + 1);
	m_holeText.SetStyle(kOutlineStyle);
	m_holeText.SetFont(kBigFont);
	m_holeText.SetColors(0, kParTopColor, kParBotColor);
	m_holeText.SetColors(1, kParOutlineTopColor, kParOutlineBotColor);
	
	int paroffx = 0;
	if(m_holeNum + 1 >= 10)
		paroffx += 16;
	
	m_parText.SetAlignment(kAlignLeft);
	m_parText.SetPosition(30 + paroffx, 20);
	m_parText.SetFormat(kIntValue, "PAR %d");
	m_parText.SetStyle(kOutlineStyle);
	m_parText.SetColors(0, kParTopColor, kParBotColor);
	m_parText.SetColors(1, kParOutlineTopColor, kParOutlineBotColor);
	
	m_remainingDistText.SetAlignment(kAlignLeft);
	m_remainingDistText.SetPosition(30 + paroffx, 36);
	m_remainingDistText.SetFormat(kIntValue, "%d yds");
	m_remainingDistText.SetStyle(kOutlineStyle);
	m_remainingDistText.SetColors(0, kBallRemainingTopColor, kBallRemainingBotColor);
	m_remainingDistText.SetColors(1, kBallRemainingOutlineTopColor, kBallRemainingOutlineBotColor);
	
	m_strokeText.SetAlignment(kAlignRight);
	m_strokeText.SetPosition(310, 20);
	m_strokeText.SetFormat(kIntValue, "Stroke %d");
	m_strokeText.SetStyle(kOutlineStyle);
	m_strokeText.SetColors(0, kParTopColor, kParBotColor);
	m_strokeText.SetColors(1, kParOutlineTopColor, kParOutlineBotColor);
	
	m_scoreText.SetAlignment(kAlignRight);
	m_scoreText.SetPosition(310, 36);
	m_scoreText.SetFormat(kSignedIntValue, "%s");
	m_scoreText.SetStyle(kOutlineStyle);
	m_scoreText.SetColors(0, kBallRemainingTopColor, kBallRemainingBotColor);
	m_scoreText.SetColors(1, kBallRemainingOutlineTopColor, kBallRemainingOutlineBotColor);
	
	m_clubDistText.SetAlignment(kAlignLeft);
	m_clubDistText.SetPosition(6, 480 - 44 - 10);
	m_clubDistText.SetFormat(kIntValue, "%d yds");
	m_clubDistText.SetStyle(kOutlineStyle);
	m_clubDistText.SetColors(0, kBallDistanceTopColor, kBallDistanceBotColor);
	m_clubDistText.SetColors(1, kBallDistanceOutlineTopColor, kBallDistanceOutlineBotColor);
	
	m_windText.SetAlignment(kAlignRight);
	m_windText.SetPosition(320 - 6, 480 - 44 - 10);
	m_windText.SetFormat(kIntValue, "%d mph");
	m_windText.SetStyle(kOutlineStyle);
	m_windText.SetColors(0, kBallDistanceTopColor, kBallDistanceBotColor);
	m_windText.SetColors(1, kBallDistanceOutlineTopColor, kBallDistanceOutlineBotColor);
	
	m_windControl.SetRect(RudeRect(480 - 64 - 64, 320 - 64, 480 - 64, 320));
	
	m_shotEncouragementText.SetAlignment(kAlignCenter);
	m_shotEncouragementText.SetRect(RudeRect(80, 0, 100, 320));
	m_shotEncouragementText.SetStyle(kOutlineStyle);
	m_shotEncouragementText.SetFont(kBigFont);
	m_shotEncouragementText.SetColors(0, kBallRemainingTopColor, kBallRemainingBotColor);
	m_shotEncouragementText.SetColors(1, kBallRemainingOutlineTopColor, kBallRemainingOutlineBotColor);
	
	m_shotDistText.SetFormat(kIntValue, "%d yds");
	m_shotDistText.SetAlignment(kAlignCenter);
	m_shotDistText.SetRect(RudeRect(430, 0, 446, 320));
	m_shotDistText.SetStyle(kOutlineStyle);
	m_shotDistText.SetColors(0, kBallRemainingTopColor, kBallRemainingBotColor);
	m_shotDistText.SetColors(1, kBallRemainingOutlineTopColor, kBallRemainingOutlineBotColor);
	
	m_shotPowerText.SetFormat(kIntValue, "%d %% Power");
	m_shotPowerText.SetAlignment(kAlignCenter);
	m_shotPowerText.SetRect(RudeRect(446, 0, 462, 320));
	m_shotPowerText.SetStyle(kOutlineStyle);
	m_shotPowerText.SetColors(0, kBallRemainingTopColor, kBallRemainingBotColor);
	m_shotPowerText.SetColors(1, kBallRemainingOutlineTopColor, kBallRemainingOutlineBotColor);
	
	m_shotAngleText.SetFormat(kIntValue, "%d %% Angle");
	m_shotAngleText.SetAlignment(kAlignCenter);
	m_shotAngleText.SetRect(RudeRect(462, 0, 478, 320));
	m_shotAngleText.SetStyle(kOutlineStyle);
	m_shotAngleText.SetColors(0, kBallRemainingTopColor, kBallRemainingBotColor);
	m_shotAngleText.SetColors(1, kBallRemainingOutlineTopColor, kBallRemainingOutlineBotColor);
	
	m_guidePowerText.SetFormat(kIntValue, "%d %%");
	m_guidePowerText.SetAlignment(kAlignCenter);
	m_guidePowerText.SetRect(RudeRect(446, 0, 462, 320));
	m_guidePowerText.SetStyle(kOutlineStyle);
	m_guidePowerText.SetColors(0, kBallDistanceTopColor, kBallDistanceBotColor);
	m_guidePowerText.SetColors(1, kBallDistanceOutlineTopColor, kBallDistanceOutlineBotColor);
						  
	// swing controls
	
	const int kBottomBarTop = 480 - 44;
	const int kBottomBarBot = 480;
	
	m_botBarBg.SetRect(RudeRect(kBottomBarTop, 0, kBottomBarBot, 320));
	m_botBarBg.SetTextures("ui_botbarbg", "ui_botbarbg");
	
	m_swingControl.SetRect(RudeRect(0,0,400,320));
	m_swingControl.SetGolfer(&m_golfer);
	
	m_swingButton.SetRect(RudeRect(kBottomBarTop, 255, kBottomBarBot, 255+61));
	m_swingButton.SetTextures("ui_swing", "ui_swing");
	
	m_moveButton.SetRect(RudeRect(kBottomBarTop, 255, kBottomBarBot, 255+61));
	m_moveButton.SetTextures("ui_move", "ui_move");
	
	m_menuButton.SetRect(RudeRect(kBottomBarTop, 190, kBottomBarBot, 190+61));
	m_menuButton.SetTextures("ui_menu", "ui_menu");
	
	m_guideIndicatorButton.SetTextures("guide", "guide");
	m_placementGuideIndicatorButton.SetTextures("guide", "guide");
	
	m_swingCamAdjust.SetRect(RudeRect(80, 0, 480 - 80, 320));
	m_swingYaw = 0.0f;
	m_swingCamYaw = 0.0f;
	
	const int kGuideAdjustSize = 20;
	m_guideAdjust.SetRect(RudeRect(240 - kGuideAdjustSize, 160 - kGuideAdjustSize, 240 + kGuideAdjustSize, 160 + kGuideAdjustSize));

	
	m_prevClubButton.SetRect(RudeRect(kBottomBarTop, 5, kBottomBarBot, 5+32));
	m_prevClubButton.SetTextures("ui_clubprev", "ui_clubprev");
	m_clubButton.SetRect(RudeRect(kBottomBarTop, 46, kBottomBarBot, 46+68));
	m_clubButton.SetTextures("ui_1wood", "ui_1wood");
	m_nextClubButton.SetRect(RudeRect(kBottomBarTop, 121, kBottomBarBot, 121+32));
	m_nextClubButton.SetTextures("ui_clubnext", "ui_clubnext");
	
	m_cameraButton.SetRect(RudeRect(0, 160-40, 44, 160+40));
	m_cameraButton.SetTextures("ui_camera", "ui_camera");
	
	if(restorestate)
		LoadState();
	else
		SetState(kStateTeePosition);
}

RBTGame::~RBTGame()
{
	RudePhysics::GetInstance()->Destroy();
}

void RBTGame::SaveState()
{
	RudeRegistry *reg = RudeRegistry::GetSingleton();
	
	tRBTGameStateSave save;
	
	save.size = sizeof(save);
	save.state = m_state;
	save.curClub = m_curClub;
	save.windDir = m_windDir;
	save.windVec = m_windVec;
	save.windSpeed = m_windSpeed;
	save.swingPower = m_swingPower;
	save.swingAngle = m_swingAngle;
	save.ballToHoleDist = m_ballToHoleDist;
	save.ball = m_ball.GetPosition();
	save.ballMaterial = m_ball.GetCurMaterial();
	
	reg->SetByte("GOLF", "GS_INGAME_STATE", &save, sizeof(save));
	
	for(int i = 0; i < m_numPlayers; i++)
	{
		RBScoreTracker *tracker = GetScoreTracker(i);
		tracker->SaveState(i);
	}
}

int RBTGame::LoadState()
{
	RudeRegistry *reg = RudeRegistry::GetSingleton();
	
	tRBTGameStateSave load;
	int loadsize = sizeof(load);
	
	if(reg->QueryByte("GOLF", "GS_INGAME_STATE", &load, &loadsize) == 0)
	{
		if(load.size != sizeof(load))
		{
			RUDE_REPORT("Failed to load game state, size mismatch!\n");
			return -1;
		}
		
		m_state = load.state;
		m_curClub = load.curClub;
		m_windDir = load.windDir;
		m_windVec = load.windVec;
		m_windSpeed = load.windSpeed;
		m_swingPower = load.swingPower;
		m_swingAngle = load.swingAngle;
		m_ballToHoleDist = load.ballToHoleDist;
		m_ball.StickAtPosition(load.ball);
		m_ball.SetCurMaterial(load.ballMaterial);
	
		RestoreState();
		
		
		return 0;
	}
	
	return -1;
}

void RBTGame::RestoreState()
{
	if(m_state == kStatePositionSwing2
	   || m_state == kStatePositionSwing3
	   || m_state == kStateWaitForSwing
	   || m_state == kStateMenu
	   || m_state == kStateExecuteSwing
	   || m_state == kStateRegardBall)
		m_state = kStatePositionSwing;
	
	if(m_state == kStateHitBall
	   || m_state == kStateFollowBall)
		m_state = kStateHitBall;
	
	m_windText.SetValue(m_windSpeed);
	m_windControl.SetWind(m_windDir, m_windSpeed);
	m_ball.SetWindSpeed(m_windVec);
	
	// update golfer renderable
	RBGolfClub *club = RBGolfClub::GetClub(m_curClub);
	m_golfer.SetSwingType(club->m_type);
	
	// tell terrain to update guide point
	m_terrain.UpdateGuidePoint(m_ball.GetPosition(), club->m_dist * 3.0f);
	
	switch(m_state)
	{
		case kStateTeePosition:
		{
			NextClub(0);
		}
			break;
		case kStatePositionSwing:
		{
			SetState(m_state);
		}
			break;
		case kStateHitBall:
		{
			// animate the golfer to the end of the swing
			m_golfer.SetForwardSwing(1.0f);
			
			// tweak some crap w/ the camera to trick it into positioning properly
			MoveAimCamera(RudeScreenVertex(0,0), RudeScreenVertex(0,0));
			FreshGuide(true);
			m_ballCamera.SetTrackMode(kHitCamera);
			m_ballCamera.NextFrame(1.0f);
		}
			break;
	}
}

void RBTGame::SetState(eRBTGameState state)
{
	// If we're at the driving range reset the ball once it stops
	if(m_holeSet == kCourseDrivingRange)
	{
		if(state == kStateRegardBall)
		{
			state = kStateTeePosition;
		}
	}
	
	RUDE_REPORT("RBTGame::SetState %d => %d\n", m_state, state);
	eRBTGameState prevstate = m_state;
	m_state = state;
	
	switch(m_state)
	{
		case kStateTeePosition:
			{
				m_curClub = 0;
				NextClub(0);
			}
			break;
		case kStatePositionSwing:
			{
				m_golfer.SetReady();
				
				m_swingHeight = 0.0f;
				
				if(prevstate == kStatePositionSwing2)
				{
					m_ballCamera.SetHeight(10.0f);
				}
				else if(prevstate == kStateExecuteSwing)
				{
				}
				else
				{
					m_swingYaw = 0.0f;
					
					FreshShotEncouragement();
					
					AutoSelectClub();
				}
				
				FreshGuide(true);
			}
			break;
		case kStatePositionSwing2:
			{
				
				if(prevstate != kStatePositionSwing3)
				{
					m_swingCamYaw = 0.0f;
					m_swingHeight = 50.0f;
				}
				
				FreshGuide();
				MoveAimCamera(RudeScreenVertex(0,0), RudeScreenVertex(0,0));
			}
			break;
		case kStatePositionSwing3:
			{
				m_placementGuidePower = 100;
				m_placementGuidePosition = m_guidePosition;
			}
			break;
		case kStateMenu:
			{
				m_menu.Reset(m_holeNum, m_holeSet);
			}
			break;
		case kStateExecuteSwing:
			{
				m_swingControl.Reset();
				m_swingHeight = 0.0f;
				FreshGuide();
			}
			break;
		case kStateWaitForSwing:
			{
				
			}
			break;
		case kStateHitBall:
			{ 
				m_swingPower = m_swingControl.GetPower();
				m_swingAngle = m_swingControl.GetAngle();
				m_ballRecorder.Reset();
			}
			break;
		case kStateFollowBall:
			{
				m_stopTimer = 0.0f;
				m_followTimer = 0.0f;
				m_ballCamera.SetTrackMode(kAfterShotCamera);
				
			}
			break;
		case kStateRegardBall:
			{
				GetScoreTracker(m_curPlayer)->AddStrokes(m_holeNum, 1);
				
				m_ballCamera.SetDesiredHeight(5.0f);
				m_ballCamera.ResetGuide(m_terrain.GetGuidePoint());
				m_ballCamera.SetTrackMode(kRegardCamera);
				
			}
			break;
		case kStateBallOutOfBounds:
			{
				// one stroke penalty
				GetScoreTracker(m_curPlayer)->AddStrokes(m_holeNum, 1);
				
				StickBallInBounds();
				
				SetState(kStateRegardBall);
			}
			break;
		case kStateBallInHole:
			{
				GetScoreTracker(m_curPlayer)->AddStrokes(m_holeNum, 1);
				
				RudeSound::GetInstance()->PlayWave(kSoundBallInHole);
				RudeSound::GetInstance()->BgmVolFade(-1.0f);
				
				m_ballCamera.SetDesiredHeight(5.0f);
				m_ballCamera.SetGuide(m_terrain.GetHole());
				m_ballCamera.SetTrackMode(kRegardCamera);
				
			}
			break;
	}
	
	
	SaveState();
}

void RBTGame::StateTeePosition(float delta)
{
	btVector3 tee = m_terrain.GetTeeBox();
	m_ball.StickAtPosition(tee);
	m_ball.SetCurMaterial(kTee);

	m_terrain.SetBallInHole(false);
	
	btVector3 ballToHole = m_terrain.GetHole() - tee;
	ballToHole.setY(0.0f);
	float yardage = ballToHole.length() / 3.0f;
	
	m_curClub = RBGolfClub::AutoSelectClub(yardage, m_ball.GetCurMaterial());
	
	int windspeed = 0;
	
	switch(m_wind)
	{
		case kCourseNoWind:
			windspeed = 0;
			break;
		case kCourseLowWind:
		{
			windspeed = rand() % 4 + 1;
		}
			break;
		case kCourseHighWind:
		{
			windspeed = rand() % 10 + 4;
		}
			break;
	}
	
	// temporarily force no wind
	windspeed = 0;
	
	m_windText.SetValue(windspeed);
	
	m_windSpeed = windspeed;
	
	btVector3 winddir(1,0,0);
	
	//m_windDir = 0;
	m_windDir = rand() % 360;
	m_windDir = (m_windDir / 180.0f) * 3.1415926f;
	
	btMatrix3x3 rmat;
	rmat.setEulerYPR(m_windDir, 0.0f, 0.0f);
	
	m_windVec = rmat * winddir;
	m_windVec *= m_windSpeed;
	
	m_windControl.SetWind(m_windDir, windspeed);
	m_ball.SetWindSpeed(m_windVec);
	
	SetState(kStatePositionSwing);
}

void RBTGame::StatePositionSwing(float delta)
{
	m_encouragementTimer -= delta;
	
	float alpha = m_encouragementTimer;
	if(alpha > 1.0f)
		alpha = 1.0f;
	m_shotEncouragementText.SetAlpha(alpha);
	
	//m_state = kStateExecuteSwing;
	
	//AutoSelectClub();
}

void RBTGame::StatePositionSwing2(float delta)
{
	m_encouragementTimer = 0.0f;
	
	m_ballGuide.NextFrame(delta);
}

void RBTGame::StateHitBall(float delta)
{
	HitBall();
	
	SetState(kStateFollowBall);
	
}

void RBTGame::StateFollowBall(float delta)
{	
	m_ballRecorder.NextFrame(delta, true);
	
	btVector3 ball = m_ball.GetPosition();
	
	// check to make sure the ball is in bounds
	
	bool inbounds = !m_terrain.IsOutOfBounds(ball);
	
	if(inbounds)
	{
		m_ballLastInBoundsPosition = ball;
	}
	else
	{
		if(m_terrain.IsOutOfBoundsAndGone(ball))
		{
			SetState(kStateBallOutOfBounds);
			return;
		}
	}
	
	// increment the follow timer and switch to follow cam
	
	m_followTimer += delta;
	
	if(m_followTimer > kFollowTimerThreshold)
	{
		// project ball forward based on velocity
		const float kBallForwardTime = 2.0f;
		btVector3 ballvel = m_ball.GetLinearVelocity();
		ballvel.setY(0);
		
		btVector3 futureball = ball + ballvel * kBallForwardTime;
		
		btVector3 placement = m_terrain.GetCameraPlacement(futureball);
		
		m_ballCamera.ResetGuide(placement);
		m_ballCamera.SetTrackMode(kPlacementCamera);
		
		
		m_followTimer = -100.0f;
	}
	else
	{
	}
	
	
	// update yardage calculation
	
	btVector3 dist = ball - m_dBall;
	dist.setY(0.0f);
	float df = dist.length();
	m_ballShotDist = df / 3.0f;
	
	btVector3 ballToHole = m_terrain.GetHole() - ball;
	ballToHole.setY(0.0f);
	m_ballToHoleDist = ballToHole.length() / 3.0f;
	
	
	// determine if the ball is stopped
	
	if(m_ball.GetStopped())
	{
		if(m_terrain.GetBallInHole())
		{
			SetState(kStateBallInHole);
			return;
		}
		
		if(!m_playedBallDissapointmentSound)
		{
			if(m_ballToHoleDist < 2.0f)
			{
				RudeSound::GetInstance()->PlayWave(kSoundMissedPutt);
				m_playedBallDissapointmentSound = true;
			}
		}
		
		const float kBallStoppedObservationTime = 2.0f;
		
		m_stopTimer += delta;
		
		if(m_stopTimer > kBallStoppedObservationTime)
		{
			SetState(kStateRegardBall);
		}
	}
}

void RBTGame::FreshShotEncouragement()
{
	if(m_ball.GetCurMaterial() == kGreen)
	{
		RBScoreTracker *tracker = GetScoreTracker(m_curPlayer);
		int strokes = tracker->GetNumStrokes(m_holeNum) + 1;
		int scoreIfShotMade = strokes - m_par;
		
		const float kEncouragementTimer = 3.0f;
		
		if(scoreIfShotMade == -2)
		{
			m_shotEncouragementText.SetText("For Eagle!");
			m_encouragementTimer = kEncouragementTimer;
		}
		else if(scoreIfShotMade == -1)
		{
			m_shotEncouragementText.SetText("For Birdie!");
			m_encouragementTimer = kEncouragementTimer;
		}
		else if(scoreIfShotMade == 0)
		{
			m_shotEncouragementText.SetText("For Par");
			m_encouragementTimer = kEncouragementTimer;
		}
		else if(scoreIfShotMade == 1)
		{
			m_shotEncouragementText.SetText("For Bogey");
			m_encouragementTimer = kEncouragementTimer;
		}
	}
	
	
}

void RBTGame::AutoSelectClub()
{
	btVector3 ballToHole = m_terrain.GetHole() - m_ball.GetPosition();
	ballToHole.setY(0.0f);
	float yardage = ballToHole.length() / 3.0f;
	
	m_curClub = RBGolfClub::AutoSelectClub(yardage, m_ball.GetCurMaterial());
	
	RBGolfClub *club = RBGolfClub::GetClub(m_curClub);
	
	// update golfer renderable
	m_golfer.SetSwingType(club->m_type);
	
	// tell terrain to update guide point
	m_terrain.UpdateGuidePoint(m_ball.GetPosition(), club->m_dist * 3.0f);
	
	// if the auto-selected club is the putter, change to the putting music
	
	if(club->m_type == kClubPutter)
	{
		m_swingControl.SetNoSwingCommentary(true);
		RudeSound::GetInstance()->PlaySong(kBGMPutting);
		RudeSound::GetInstance()->BgmVol(1.0f);
	}
	else
	{
		m_swingControl.SetNoSwingCommentary(false);
		RudeSound::GetInstance()->BgmVolFade(-0.2f);
	}
	
	NextClub(0);
}

void RBTGame::NextClub(int n)
{
	if(n > 0)
		m_curClub = RBGolfClub::NextClub(m_curClub, m_ball.GetCurMaterial());
	if(n < 0)
		m_curClub = RBGolfClub::PrevClub(m_curClub, m_ball.GetCurMaterial());
	
	RBGolfClub *club = RBGolfClub::GetClub(m_curClub);
	
	m_clubButton.SetTextures(club->m_textureName, club->m_textureName);
	
	m_golfer.SetSwingType(club->m_type);
	
	FreshGuide();
}

void RBTGame::MovePosition(const RudeScreenVertex &p, const RudeScreenVertex &dist)
{
	//printf("move %d %d\n", p.m_x, p.m_y);
	
	const float kYawDamping = 0.002f;
	
	if(!m_moveHeight && dist.m_x > 30)
	{
		m_moveGuide = true;
	}
	else if(!m_moveGuide && dist.m_y > 30)
	{
		m_moveHeight = true;
	}
		
		
	if(m_moveGuide)
	{
		float dx = p.m_x;
		m_swingYaw += (dx * kYawDamping);
	}
	
	if(m_moveHeight)
	{
		const float kHeightDamping = 0.004f;
		const float kMaxHeight = 1.0f;
		float dy = p.m_y;
		m_swingHeight -= (dy * kHeightDamping);
		if(m_swingHeight < 0.0f)
			m_swingHeight = 0.0f;
		if(m_swingHeight > kMaxHeight)
			m_swingHeight = kMaxHeight;
	}

	FreshGuide();
}

void RBTGame::MoveAimCamera(const RudeScreenVertex &p, const RudeScreenVertex &dist)
{
	//printf("move %d %d\n", p.m_x, p.m_y);
	
	const float kYawDamping = 0.006f;
	
	if(dist.m_x > 30)
	{
		float dx = p.m_x;
		m_swingCamYaw += (dx * kYawDamping);
	}
	
	const float kHeightDamping = 0.004f;
	const float kMaxHeight = 1.0f;
	float dy = p.m_y;
	m_swingHeight -= (dy * kHeightDamping);
	if(m_swingHeight < 0.0f)
		m_swingHeight = 0.0f;
	if(m_swingHeight > kMaxHeight)
		m_swingHeight = kMaxHeight;
	
	m_ballCamera.SetYaw(m_swingCamYaw);
	m_ballCamera.SetDesiredHeight(m_swingHeight);
}

void RBTGame::StickBallInBounds()
{
	// assumes the ball is just barely out of bounds!
	
	const float kTestLen = 10.0f;
	const float kTestIncrement = 0.5f;
	
	float scores[8];
	float bestscore = 0.0f;
	int bestscorer = -1;
	btVector3 bestpos;
	
	for(int i = 0; i < 8; i++)
	{
		scores[i] = 0.0f;
		
		btVector3 v(1.0f, 0.0f, 0.0f);
		
		btMatrix3x3 rotmat;
		rotmat.setEulerYPR(i * 0.25f * 3.1415926f, 0.0f, 0.0f);
		
		v = rotmat * v;
		
		for(float f = kTestIncrement; f < kTestLen; f += kTestIncrement)
		{
			btVector3 testpos = m_ballLastInBoundsPosition + v * f;
			bool inbounds = !m_terrain.IsOutOfBounds(testpos);
		
			if(inbounds)
				scores[i] += 1.0f;
			
			if(scores[i] > bestscore)
			{
				bestscore = scores[i];
				bestscorer = i;
				
				bestpos = testpos;
			}
		}
	}
	
	if(bestscorer < 0)
	{
		m_ball.StickAtPosition(m_ballLastInBoundsPosition);
		return;
	}
	
	m_ball.StickAtPosition(bestpos);
	
}

void RBTGame::FreshGuide(bool firstTime)
{
	
	btVector3 ball = m_ball.GetPosition();
	btVector3 guide = m_terrain.GetGuidePoint();
	btVector3 aimvec = guide - ball;
	
	btMatrix3x3 mat;
	mat.setEulerYPR(m_swingYaw, 0.0f, 0.0f);
	btVector3 newGuide = mat * aimvec;
	newGuide.setY(0);
	newGuide.normalize();
	newGuide *= RBGolfClub::GetClub(m_curClub)->m_dist * 3.0f;
	newGuide += ball;
	//btVector3 newGuide = btVector3(0,0,-100) + ball;
	
	bool fullrez = false;
	if(m_state == kStatePositionSwing2)
	{
		fullrez = true;
	}
	
	
	m_golfer.SetPosition(ball, newGuide);
	
	m_ballGuide.SetGuide(newGuide, fullrez);
	newGuide = m_ballGuide.GetLastGuidePoint();
	
	if(firstTime)
		m_ballCamera.ResetGuide(newGuide);
	
	m_ballCamera.SetGuide(newGuide);
	m_ballCamera.SetDesiredHeight(m_swingHeight);
	
	if(m_state == kStatePositionSwing2)
	{
		m_ballCamera.SetTrackMode(kAimCamera);
	}
	else
	{
		RBGolfClub *club = RBGolfClub::GetClub(m_curClub);
		
		if(club->m_type == kClubPutter)
			m_ballCamera.SetTrackMode(kPuttCamera);
		else
			m_ballCamera.SetTrackMode(kHitCamera);
	}
	
	m_dBall = ball;
	m_guidePosition = newGuide;
}

void RBTGame::HitBall()
{

	
	const float kMaxAngleModifier = 2.5f;
	float angleModifier = m_swingAngle * (kMaxAngleModifier / 180.0f) * 3.1415926f;
	
	RBGolfClub *club = RBGolfClub::GetClub(m_curClub);
	
	RudeSound::GetInstance()->PlayWave(club->m_swingsound);
	
	
	
	float loft = club->m_loft;
	loft = (loft / 180.0f) * 3.1415926f;
	
	btVector3 ball = m_ball.GetPosition();
	btVector3 guide = m_terrain.GetGuidePoint();
	btVector3 aimvec = guide - ball;
	aimvec.setY(0);
	aimvec.normalize();
	
	btMatrix3x3 guidemat;
	guidemat.setEulerYPR(m_swingYaw + angleModifier, 0.0f, 0.0f);
	aimvec = guidemat * aimvec;
	
	aimvec.setY(tan(loft));
	aimvec.normalize();
	
	//aimvec = btVector3(0,0.9,-0.1);
	//aimvec.normalize();
	
	btVector3 linvel = aimvec * club->m_power * m_swingPower;
	
	
	btVector3 upvec(0,1,0);
	btVector3 rightvec = aimvec.cross(upvec);
	
	const float kMaxSliceModifier = 20.0f;
	
	btVector3 spinForce = rightvec * m_swingAngle * kMaxSliceModifier;
	
	//printf("spinForce = %f %f %f\n", spinForce.x(), spinForce.y(), spinForce.z());
	
	m_ballLastInBoundsPosition = ball;
	
	m_ball.HitBall(linvel, spinForce);
	
}


void RBTGame::AdjustGuide()
{
	if(!m_guideScreenCalc)
		return;
	
	btVector3 ball = m_ball.GetPosition();
	btVector3 guide = m_terrain.GetGuidePoint();
	btVector3 aimvec = guide - ball;
	
	//printf("screen point: %d %d\n", m_guideScreenPoint.m_x, m_guideScreenPoint.m_y);
	
	btVector3 screenp(m_guideScreenPoint.m_x, m_guideScreenPoint.m_y, 0.0f);
	
	btVector3 worldp = RGL.InverseProject(screenp);
	
	btVector3 eyep = RGL.GetEye();
	
	btVector3 result;
	bool found = m_terrain.CastToTerrain(eyep, worldp, result);
	
	if(found)
	{
		btVector3 newaimvec = result - ball;
		newaimvec.setY(0.0f);
		float distance = newaimvec.length() / 3.0f;
		newaimvec.normalize();
		
		aimvec.setY(0.0f);
		aimvec.normalize();
		
		btVector3 yup(0,1,0);
		btVector3 xdir = aimvec.cross(yup);
		
		float inx = xdir.dot(newaimvec);
		
		float angle = aimvec.dot(newaimvec);
		
		m_swingYaw = acos(angle);
		
		if(inx < 0.0f)
			m_swingYaw *= -1.0f;
		
		//printf("angle %f yaw %f\n", angle, m_swingYaw);
		
		//FreshGuide();
		
		//m_ballGuide.SetGuide(result, true);
		
		m_placementGuidePosition = result;
		
		RBGolfClub *club = RBGolfClub::GetClub(m_curClub);
	
		float power = 100.0f * distance / club->m_dist;
		m_placementGuidePower = power;
	}
	else
	{
		m_placementGuidePower = 0;
	}
	
	//printf("guide %f %f %f\n", m_guidePosition.x(), m_guidePosition.y(), m_guidePosition.z());
	
	
	m_guideScreenCalc = false;
}

void RBTGame::NextFrame(float delta)
{
	RUDE_PERF_START(kPerfRBTGameNextFrame);
	
	if(gDebugCamera != gDebugCameraPrev)
	{
		gDebugCameraPrev = gDebugCamera;
		
		if(gDebugCamera)
		{
			m_debugCamera.SetPos(m_ball.GetPosition() + btVector3(0,10,0));
			m_curCamera = &m_debugCamera;
		}
		else
			m_curCamera = &m_ballCamera;
	}
	
	if(gDebugFinishHole)
	{
		m_terrain.SetBallInHole(true);
		SetState(kStateBallInHole);
		gDebugFinishHole = false;
	}
	
	
	RudePhysics::GetInstance()->NextFrame(delta);
	
	switch(m_state)
	{
		case kStateTeePosition:
		{
			StateTeePosition(delta);
			
			btVector3 ball = m_ball.GetPosition();
			btVector3 ballToHole = m_terrain.GetHole() - ball;
			ballToHole.setY(0.0f);
			m_ballToHoleDist = ballToHole.length() / 3.0f;
		}
			break;
		case kStatePositionSwing:
			StatePositionSwing(delta);
			m_ballRecorder.NextFrame(delta, false);
			break;
		case kStatePositionSwing2:
			StatePositionSwing2(delta);
			m_ballRecorder.NextFrame(delta, false);
			break;
		case kStateMenu:
			m_menu.NextFrame(delta);
			
			if(m_menu.Done())
			{
				if(m_menu.GetResult() == kMenuResume)
					SetState(kStatePositionSwing);
				else if(m_menu.GetResult() == kMenuQuit)
				{
					m_result = kResultQuit;
					m_done = true;
				}
					
			}
			
			break;
		case kStateExecuteSwing:
			
			m_swingControl.NextFrame(delta);
			
			if(m_swingControl.WillSwing())
				SetState(kStateWaitForSwing);
			
			break;
		case kStateWaitForSwing:
		{
			m_swingControl.NextFrame(delta);
			
			if(m_golfer.HasSwung())
				SetState(kStateHitBall);
		}
			break;
		case kStateHitBall:
		{
			m_swingControl.NextFrame(delta);
			
			StateHitBall(delta);
			
			//btVector3 ball = m_ball.GetPosition();
			//printf("ball: %f %f %f\n", ball.x(), ball.y(), ball.z());
		}
			break;
		case kStateFollowBall:
		{
			m_swingControl.NextFrame(delta);
			
			StateFollowBall(delta);
		}
		
			break;
			
		case kStateRegardBall:
			
			m_ballRecorder.NextFrame(delta, false);
			
			break;
			
		default:
			break;
	}

	//RGLD.DebugDrawLine(m_guidePosition, m_guidePosition + btVector3(0,10,0));
	
	m_golfer.NextFrame(delta);
	m_windControl.NextFrame(delta);
	
	m_ball.NextFrame(delta);
	m_curCamera->NextFrame(delta);
	
	
	RUDE_PERF_STOP(kPerfRBTGameNextFrame);
}

void RBTGame::RenderCalcOrthoDrawPositions()
{
	switch(m_state)
	{
		case kStatePositionSwing:
		case kStatePositionSwing2:
		case kStatePositionSwing3:
			{

				m_guidePositionScreenSpace = RGL.Project(m_guidePosition);
				
				if(m_state == kStatePositionSwing)
				{
					m_guidePositionScreenSpace.setX(160.0f);
				}
				
				if(m_state == kStatePositionSwing3)
				{
					m_placementGuidePositionScreenSpace = RGL.Project(m_placementGuidePosition);
					
				}
			}
			break;
	}
}

void RBTGame::RenderGuide(float aspect)
{
	
	{
		const int kGuideSize = 32;
		RudeRect r(
				   m_guidePositionScreenSpace.y() - kGuideSize,
				   m_guidePositionScreenSpace.x() - kGuideSize,
				   m_guidePositionScreenSpace.y() + kGuideSize,
				   m_guidePositionScreenSpace.x() + kGuideSize
				   );
		
		m_guideIndicatorButton.SetRect(r);
		m_guideIndicatorButton.Render();
	}
	
	if(m_state == kStatePositionSwing3)
	{
		{
			const int kGuideSize = 32;
			RudeRect r(
					   m_placementGuidePositionScreenSpace.y() - kGuideSize,
					   m_placementGuidePositionScreenSpace.x() - kGuideSize,
					   m_placementGuidePositionScreenSpace.y() + kGuideSize,
					   m_placementGuidePositionScreenSpace.x() + kGuideSize
					   );
			
			m_placementGuideIndicatorButton.SetRect(r);
			m_placementGuideIndicatorButton.Render();
			
		}
		
		{
			const int kTextSize = 16;
			const int kTextOffset = -58;
			RudeRect r(
					   m_placementGuidePositionScreenSpace.y() - kTextSize + kTextOffset,
					   m_placementGuidePositionScreenSpace.x() - kTextSize,
					   m_placementGuidePositionScreenSpace.y() + kTextSize + kTextOffset,
					   m_placementGuidePositionScreenSpace.x() + kTextSize
					   );
			
			m_guidePowerText.SetRect(r);
			m_guidePowerText.SetValue(m_placementGuidePower);
			m_guidePowerText.Render();
		}
	}
	else if(m_state == kStatePositionSwing2)
	{
		const int kTextSize = 16;
		const int kTextOffset = -38;
		RudeRect r(
			m_guidePositionScreenSpace.y() - kTextSize + kTextOffset,
			m_guidePositionScreenSpace.x() - kTextSize,
			m_guidePositionScreenSpace.y() + kTextSize + kTextOffset,
			m_guidePositionScreenSpace.x() + kTextSize
			);
		
		m_guidePowerText.SetRect(r);
		m_guidePowerText.SetValue(100);
		m_guidePowerText.Render();
	}
	
}



void RBTGame::RenderShotInfo(bool showShotDistance, bool showClubInfo)
{
	
	if(showShotDistance)
	{
		
		m_shotDistText.SetValue(m_ballShotDist);
		m_shotPowerText.SetValue(m_swingPower * 100.0f);
		
		if(m_swingAngle > 0.0f)
			m_shotAngleText.SetFormat(kIntValue, "%d %% Slice");
		else
			m_shotAngleText.SetFormat(kIntValue, "%d %% Hook");
		
		m_shotAngleText.SetValue(m_swingAngle * 100.0f);
		
		m_shotDistText.Render();
		m_shotPowerText.Render();
		m_shotAngleText.Render();
	}
	
	if(showClubInfo)
	{
		RBGolfClub *club = RBGolfClub::GetClub(m_curClub);
		m_clubDistText.SetValue(club->m_dist);
		m_clubDistText.Render();
	}
	
	m_parText.SetValue(m_par);
	m_remainingDistText.SetValue(m_ballToHoleDist);
	
	if(m_state == kStateBallInHole)
	{
		m_scoreText.SetValue(GetScoreTracker(m_curPlayer)->GetScore(m_holeSet, m_holeNum, true));
		m_strokeText.SetValue(GetScoreTracker(m_curPlayer)->GetNumStrokes(m_holeNum));
	}
	else
	{
		m_scoreText.SetValue(GetScoreTracker(m_curPlayer)->GetScore(m_holeSet, m_holeNum, false));
		m_strokeText.SetValue(GetScoreTracker(m_curPlayer)->GetNumStrokes(m_holeNum) + 1);
	}
	
	if(m_holeSet != kCourseDrivingRange)
	{
		m_holeText.Render();
		m_strokeText.Render();
		m_scoreText.Render();
		m_parText.Render();
		m_remainingDistText.Render();
	}
	

}

void RBTGame::Render(float aspect)
{
	RUDE_PERF_START(kPerfRBTGameRender);
	
	RGL.SetViewport(0, 0, 480, 320);

	m_curCamera->SetView(aspect);
	RGL.LoadIdentity();
	
	AdjustGuide();
	
	m_skybox.Render();
	
	RGL.Enable(kDepthTest, true);
	m_terrain.Render();
	
	if(m_state == kStateFollowBall)
	{
		m_ballRecorder.RenderTracers();
	}
	
	if(gDebugCamera)
	{
		m_ballRecorder.RenderRecords();
	}
	
	RUDE_PERF_STOP(kPerfRBTGameRender);
	
	
	RGL.Enable(kDepthTest, true);
	m_golfer.Render();
	
	
	//m_ballRecorder.RenderRecords();
	
	
	//RGL.Enable(kDepthTest, false);
	m_ball.Render();
	
	
	
	if(m_state == kStatePositionSwing2 || m_state == kStatePositionSwing3)
	{
		RGL.LoadIdentity();
		m_ballGuide.Render();
	}
	
	RGL.LoadIdentity();
	
	RenderCalcOrthoDrawPositions();
	
	
	
	RGL.Enable(kDepthTest, false);
	RGLD.RenderDebug();
	
	if(m_state == kStatePositionSwing
	   || m_state == kStatePositionSwing2
	   || m_state == kStatePositionSwing3
	   || m_state == kStateExecuteSwing)
	{
		m_windControl.Render();
	}
	
	RGL.SetViewport(0, 0, 480, 320);
	RGL.Ortho(0.0f, 0.0f, 0.0f, 320.0f, 480.0f, 100.0f);
	RGL.LoadIdentity();
	RGL.Enable(kBackfaceCull, false);
	RGL.Enable(kDepthTest, false);
	
	switch(m_state)
	{
		case kStatePositionSwing:
		case kStatePositionSwing2:
		case kStatePositionSwing3:
			RenderGuide(aspect);
			
			m_botBarBg.Render();
			m_menuButton.Render();
			m_swingButton.Render();
			m_nextClubButton.Render();
			m_prevClubButton.Render();
			m_clubButton.Render();
			m_cameraButton.Render();
			RenderShotInfo(false, true);
			m_windText.Render();
			
			if(m_encouragementTimer > 0.0f)
			{
				m_shotEncouragementText.Render();
			}
			break;
		case kStateMenu:
			m_menu.Render(aspect);
			break;
			
		case kStateExecuteSwing:
		case kStateWaitForSwing:
			m_botBarBg.Render();
			m_moveButton.Render();
			m_swingControl.Render();
			m_clubButton.Render();
			RenderShotInfo(false, true);
			m_windText.Render();
			break;
			
		case kStateHitBall:
		case kStateFollowBall:
			//RenderBallFollowInfo(false);
			RenderShotInfo(true, false);
			
			m_swingControl.Render();
			
			break;
			
		case kStateRegardBall:
			//RenderBallFollowInfo(true);
			RenderShotInfo(true, false);
			
			//RudeFontManager::GetFont(kDefaultFont)->Printf(0.0f, 440.0f, 0.0f, FONT_ALIGN_LEFT, 0xFFFFFFFF, 0xFFFFFFFF, "POWER: %.0f%%", m_swingPower * 100.0f);
			
			break;
			
		case kStateBallInHole:
			m_scoreControl.Render();
			RenderShotInfo(false, false);
			break;
	}
	
	
	
	
}

void RBTGame::LaunchBall()
{
	btVector3 &eye = m_curCamera->GetPos();	
	btVector3 &lookAt = m_curCamera->GetLookAt();
	
	btVector3 lookdir = lookAt - eye;
	lookdir.normalize();
	
	m_ball.SetPosition(eye);
	//m_ball.SetForce(lookdir * 1000);
}


void RBTGame::KeyDown(RBKey k)
{

}

void RBTGame::KeyUp(RBKey k)
{
}

void RBTGame::StylusDown(RudeScreenVertex &p)
{

}

void RBTGame::StylusUp(RudeScreenVertex &p)
{

}

void RBTGame::StylusMove(RudeScreenVertex &p)
{

}


void RBTGame::TouchDown(RudeTouch *rbt)
{
	if(gDebugCamera)
	{
		m_debugCamera.TouchDown(rbt);
		return;
	}
	
	eSoundEffect sfx = kSoundNone;
	
	switch(m_state)
	{
		case kStatePositionSwing:
		case kStatePositionSwing2:
			
			if(m_state == kStatePositionSwing2)
			{
				if(m_guideAdjust.TouchDown(rbt))
				{
					SetState(kStatePositionSwing3);
					break;
				}
			}
			
			m_moveGuide = false;
			m_moveHeight = false;
			m_swingButton.TouchDown(rbt);
			m_swingCamAdjust.TouchDown(rbt);
			m_menuButton.TouchDown(rbt);
			if(m_prevClubButton.TouchDown(rbt))
			{
				NextClub(-1);
				sfx = kSoundUIClickLow;
			}
			if(m_nextClubButton.TouchDown(rbt))
			{
				NextClub(1);
				sfx = kSoundUIClickHi;
			}
			if(m_cameraButton.TouchDown(rbt))
			{
				if(m_state == kStatePositionSwing)
				{
					SetState(kStatePositionSwing2);
					sfx = kSoundUIClickHi;
				}
				else
				{
					SetState(kStatePositionSwing);
					sfx = kSoundUIClickLow;
				}
			}
			break;
		case kStateMenu:
			m_menu.TouchDown(rbt);
			break;
		case kStateExecuteSwing:
			m_swingControl.TouchDown(rbt);
			m_moveButton.TouchDown(rbt);
			break;
		case kStateFollowBall:
			m_swingButton.TouchDown(rbt);
			break;
	}
	
	RudeSound::GetInstance()->PlayWave(sfx);
}

void RBTGame::TouchMove(RudeTouch *rbt)
{
	if(gDebugCamera)
	{
		m_debugCamera.TouchMove(rbt);
		return;
	}
	
	switch(m_state)
	{
		case kStatePositionSwing:
			if(m_swingCamAdjust.TouchMove(rbt))
				MovePosition(m_swingCamAdjust.GetMoveDelta(), m_swingCamAdjust.GetDistanceTraveled());
			break;
		case kStatePositionSwing2:
			if(m_swingCamAdjust.TouchMove(rbt))
				MoveAimCamera(m_swingCamAdjust.GetMoveDelta(), m_swingCamAdjust.GetDistanceTraveled());
			break;
		case kStatePositionSwing3:
			if(m_guideAdjust.TouchMove(rbt))
			{
				m_guideScreenPoint = m_guideAdjust.GetHitMove();
				m_guideScreenCalc = true;
				//AdjustGuide(m_guideAdjust.GetHitMove());
			}
			break;
		case kStateMenu:
			m_menu.TouchMove(rbt);
			break;
		case kStateExecuteSwing:
			m_swingControl.TouchMove(rbt);
			break;
	}
}

void RBTGame::TouchUp(RudeTouch *rbt)
{
	if(gDebugCamera)
	{
		//if(rbt->m_location.m_y < 240)
		//	LaunchBall();
		
		m_debugCamera.TouchUp(rbt);
		
		return;
	}
	
	eSoundEffect sfx = kSoundNone;
	
	switch(m_state)
	{
		case kStatePositionSwing:
		case kStatePositionSwing2:
			if(m_swingButton.TouchUp(rbt))
			{
				SetState(kStateExecuteSwing);
				sfx = kSoundUIClickHi;
			}
			m_swingCamAdjust.TouchUp(rbt);
			m_nextClubButton.TouchUp(rbt);
			m_prevClubButton.TouchUp(rbt);
			
			if(m_menuButton.TouchUp(rbt))
			{
				SetState(kStateMenu);
				sfx = kSoundUIClickHi;
			}
			break;
		case kStatePositionSwing3:
			m_guideAdjust.TouchUp(rbt);
			SetState(kStatePositionSwing2);
			break;
		case kStateMenu:
			m_menu.TouchUp(rbt);
			break;
		case kStateExecuteSwing:
			if(m_swingControl.TouchUp(rbt))
			{
				
				
			}
			if(m_moveButton.TouchUp(rbt))
			{
				SetState(kStatePositionSwing);
				sfx = kSoundUIClickHi;
			}
			break;
		case kStateFollowBall:
			if(m_swingButton.TouchUp(rbt))
			{
				if(m_followTimer > 0.0f)
					m_followTimer = kFollowTimerThreshold;
			}
			break;
		case kStateRegardBall:
			SetState(kStatePositionSwing);
			break;
		case kStateBallInHole:
			//SetState(kStateTeePosition);
			m_result = kResultComplete;
			m_done = true;
			break;
	}
	
	RudeSound::GetInstance()->PlayWave(sfx);

}

void RBTGame::Pause()
{

}


