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
#include "RudePhysics.h"
#include "RudeTweaker.h"
#include "RudeSound.h"
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>

#include "btTransform.h"

bool gDebugCamera = false;
RUDE_TWEAK(DebugCamera, kBool, gDebugCamera);
bool gDebugCameraPrev = false;

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

RBTGame::RBTGame()
{	
	
	
	RudePhysics::GetInstance()->Init();
		
	m_terrain.Load("parfive");
	m_par = 5;
	m_stroke = 1;
	m_score = 0;
	
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
	
	
	// stroke/status controls
	
	m_parText.SetAlignment(kAlignLeft);
	m_parText.SetPosition(10, 20);
	m_parText.SetFormat(kIntValue, "PAR %d");
	m_parText.SetStyle(kOutlineStyle);
	m_parText.SetColors(0, kParTopColor, kParBotColor);
	m_parText.SetColors(1, kParOutlineTopColor, kParOutlineBotColor);
	
	m_remainingDistText.SetAlignment(kAlignLeft);
	m_remainingDistText.SetPosition(10, 36);
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
	
						  
	// swing controls
	
	m_swingControl.SetRect(RudeRect(0,0,400,320));
	m_swingControl.SetGolfer(&m_golfer);
	
	m_swingButton.SetRect(RudeRect(480 - 60, 180, 480, 320));
	m_swingButton.SetTextures("swing", "swing");
	
	m_moveButton.SetRect(RudeRect(480 - 60, 180, 480, 320));
	m_moveButton.SetTextures("move", "move");
	
	m_guideIndicatorButton.SetTextures("guide", "guide");
	
	m_swingCamAdjust.SetRect(RudeRect(80, 0, 480 - 80, 320));
	m_swingYaw = 0.0f;
	m_swingCamYaw = 0.0f;
	
	m_prevClubButton.SetRect(RudeRect(420, 0, 480, 30));
	m_prevClubButton.SetTextures("clubprev", "clubprev");
	m_clubButton.SetRect(RudeRect(430, 45, 480, 130));
	m_clubButton.SetTextures("1wood", "1wood");
	m_nextClubButton.SetRect(RudeRect(420, 140, 480, 170));
	m_nextClubButton.SetTextures("clubnext", "clubnext");
	
	m_cameraButton.SetRect(RudeRect(0, 160-40, 60, 160+40));
	m_cameraButton.SetTextures("camera", "camera");
	
	SetState(kStateTeePosition);
}

RBTGame::~RBTGame()
{

}

void RBTGame::SetState(eRBTGameState state)
{
	RUDE_REPORT("SetState %d => %d\n", m_state, state);
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
				
				if(prevstate == kStateExecuteSwing)
					m_swingHeight = 0.0f;
				else if(prevstate != kStatePositionSwing2)
				{
					m_swingYaw = 0.0f;
					m_swingHeight = 0.0f;
					
					AutoSelectClub();
				}
				
				FreshGuide();
			}
			break;
		case kStatePositionSwing2:
			{
				m_swingCamYaw = 0.0f;
				FreshGuide();
				MoveAimCamera(RudeScreenVertex(0,0), RudeScreenVertex(0,0));
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
				m_golfer.SetForwardSwing(m_swingControl.GetPower());
			}
			break;
		case kStateHitBall:
			{ 
				m_ballRecorder.Reset();
			}
			break;
		case kStateFollowBall:
			{
				m_stopTimer = 0.0f;
				m_followTimer = 0.0f;
				m_ballCamera.Track(kAfterShotCamera, btVector3(0,0,0), 0.0f);
				
			}
			break;
		case kStateRegardBall:
			{
				m_stroke++;
				m_ballCamera.Track(kRegardCamera, m_terrain.GetGuidePoint(m_ball.GetPosition()), 5.0f);
			}
			break;
		case kStateBallOutOfBounds:
			{
				StickBallInBounds();
				
				m_stroke++;
				
				SetState(kStateRegardBall);
			}
			break;
		case kStateBallInHole:
			{
				RudeSound::GetInstance()->PlayWave(kSoundBallInHole);
				m_ballCamera.Track(kRegardCamera, m_terrain.GetHole(), 5.0f);
			}
			break;
	}
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
	
	SetState(kStatePositionSwing);
}

void RBTGame::StatePositionSwing(float delta)
{
	//m_state = kStateExecuteSwing;
	
	//AutoSelectClub();
}

void RBTGame::StatePositionSwing2(float delta)
{
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
		
		m_ballCamera.Track(kPlacementCamera, placement, 0.0f);
		
		
		m_followTimer = -100.0f;
		
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
		
		const float kBallStoppedObservationTime = 2.5f;
		
		m_stopTimer += delta;
		
		if(m_stopTimer > kBallStoppedObservationTime)
		{
			SetState(kStateRegardBall);
		}
	}
}

void RBTGame::AutoSelectClub()
{
	btVector3 ballToHole = m_terrain.GetHole() - m_ball.GetPosition();
	ballToHole.setY(0.0f);
	float yardage = ballToHole.length() / 3.0f;
	
	m_curClub = RBGolfClub::AutoSelectClub(yardage, m_ball.GetCurMaterial());
	
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
		const float kHeightDamping = 0.3f;
		const float kMaxHeight = 75.0f;
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
	
	const float kHeightDamping = 0.3f;
	const float kMaxHeight = 75.0f;
	float dy = p.m_y;
	m_swingHeight -= (dy * kHeightDamping);
	if(m_swingHeight < 0.0f)
		m_swingHeight = 0.0f;
	if(m_swingHeight > kMaxHeight)
		m_swingHeight = kMaxHeight;
	
	m_ballCamera.SetYaw(m_swingCamYaw);
	m_ballCamera.SetHeight(m_swingHeight);
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

void RBTGame::FreshGuide()
{
	
	btVector3 ball = m_ball.GetPosition();
	btVector3 guide = m_terrain.GetGuidePoint(ball);
	btVector3 aimvec = guide - ball;
	
	btMatrix3x3 mat;
	mat.setEulerYPR(m_swingYaw, 0.0f, 0.0f);
	btVector3 newGuide = mat * aimvec;
	newGuide.setY(0);
	newGuide.normalize();
	newGuide *= RBGolfClub::GetClub(m_curClub)->m_dist * 3.0f;
	newGuide += ball;
	//btVector3 newGuide = btVector3(0,0,-100) + ball;
	
	eTrackMode cammode = kHitCamera;
	bool fullrez = false;
	if(m_state == kStatePositionSwing2)
	{
		fullrez = true;
		cammode = kAimCamera;
	}
	
	
	m_golfer.SetPosition(ball, newGuide);
	
	m_ballGuide.SetGuide(newGuide, fullrez);
	newGuide = m_ballGuide.GetLastGuidePoint();
	
	m_ballCamera.Track(cammode, newGuide, m_swingHeight);
	
	
	m_dBall = ball;
	m_guidePosition = newGuide;
}

void RBTGame::HitBall()
{
	
	m_swingPower = m_swingControl.GetPower();
	m_swingAngle = m_swingControl.GetAngle();

	
	const float kMaxAngleModifier = 2.5f;
	float angleModifier = m_swingAngle * (kMaxAngleModifier / 180.0f) * 3.1415926f;
	
	RBGolfClub *club = RBGolfClub::GetClub(m_curClub);
	
	RudeSound::GetInstance()->PlayWave(club->m_swingsound);
	
	
	
	float loft = club->m_loft;
	loft = (loft / 180.0f) * 3.1415926f;
	
	btVector3 ball = m_ball.GetPosition();
	btVector3 guide = m_terrain.GetGuidePoint(ball);
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



void RBTGame::NextFrame(float delta)
{
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
		case kStateExecuteSwing:
			m_swingControl.NextFrame(delta);
			break;
		case kStateWaitForSwing:
		{
			if(m_golfer.HasSwung())
				SetState(kStateHitBall);
		}
			break;
		case kStateHitBall:
		{
			StateHitBall(delta);
			//btVector3 ball = m_ball.GetPosition();
			//printf("ball: %f %f %f\n", ball.x(), ball.y(), ball.z());
		}
			break;
		case kStateFollowBall:
			StateFollowBall(delta);
		
			break;
			
		case kStateRegardBall:
			
			m_ballRecorder.NextFrame(delta, false);
			
			break;
			
		default:
			break;
	}

	//RGLD.DebugDrawLine(m_guidePosition, m_guidePosition + btVector3(0,10,0));
	
	m_golfer.NextFrame(delta);
	
	
	m_ball.NextFrame(delta);
	m_curCamera->NextFrame(delta);
	
}

void RBTGame::RenderCalcOrthoDrawPositions()
{
	switch(m_state)
	{
		case kStatePositionSwing:
		case kStatePositionSwing2:
			{

				m_guidePositionScreenSpace = RGL.Project(m_guidePosition);
				
				//m_guidePositionScreenSpace.setX((int) m_guidePositionScreenSpace.x());
				//m_guidePositionScreenSpace.setY((int) m_guidePositionScreenSpace.y());
			}
			break;
	}
}

void RBTGame::RenderGuide(float aspect)
{
	
	
	//m_guidePositionScreenSpace.setX(m_guidePositionScreenSpace.x() * 160.0f + 160.0f);
	//m_guidePositionScreenSpace.setY(480.0f - (m_guidePositionScreenSpace.y() * 240.0f + 240.0f));
	//m_guidePositionScreenSpace.setY(480.0f - m_guidePositionScreenSpace.y());
	
	printf("Guide: %f %f %f\n", m_guidePositionScreenSpace.x(), m_guidePositionScreenSpace.y(), m_guidePositionScreenSpace.z());
	
	
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

void RBTGame::RenderBallFollowInfo(bool showDistToHole)
{
	const float kDistY = 430.0f;
	const float kRemainDistY = 450.0f;
	
	RudeFontManager::GetFont(kDefaultFontOutline)->Printf(160.0f, kDistY, 0.0f, FONT_ALIGN_CENTER, kBallDistanceOutlineTopColor, kBallDistanceOutlineBotColor, "%.2f yds", m_ballShotDist);
	RudeFontManager::GetFont(kDefaultFont)->Printf(160.0f, kDistY, 0.0f, FONT_ALIGN_CENTER, kBallDistanceTopColor, kBallDistanceBotColor, "%.2f yds", m_ballShotDist);
	
	if(showDistToHole)
	{
		RudeFontManager::GetFont(kDefaultFontOutline)->Printf(160.0f, kRemainDistY, 0.0f, FONT_ALIGN_CENTER, kBallRemainingOutlineTopColor, kBallRemainingOutlineBotColor, "%.2f yds Remaining", m_ballToHoleDist);
		RudeFontManager::GetFont(kDefaultFont)->Printf(160.0f, kRemainDistY, 0.0f, FONT_ALIGN_CENTER, kBallRemainingTopColor, kBallRemainingBotColor, "%.2f yds Remaining", m_ballToHoleDist);
		
	}
}

void RBTGame::RenderShotInfo(bool showShotDistance, bool showClubInfo)
{
	const float kParY = 20.0f;
	const float kRemainDistY = 36.0f;
	
	const float kClubDistY = 430.0f;
	const float kDistY = 430.0f;
	const float kPowerY = 446.0f;
	const float kAngleY = 462.0f;
	
	if(showShotDistance)
	{
		RudeFontManager::GetFont(kDefaultFontOutline)->Printf(160.0f, kDistY, 0.0f, FONT_ALIGN_CENTER, kBallDistanceOutlineTopColor, kBallDistanceOutlineBotColor, "%.2f yds", m_ballShotDist);
		RudeFontManager::GetFont(kDefaultFont)->Printf(160.0f, kDistY, 0.0f, FONT_ALIGN_CENTER, kBallDistanceTopColor, kBallDistanceBotColor, "%.2f yds", m_ballShotDist);
		
		RudeFontManager::GetFont(kDefaultFontOutline)->Printf(160.0f, kPowerY, 0.0f, FONT_ALIGN_CENTER, kBallDistanceOutlineTopColor, kBallDistanceOutlineBotColor, "%.0f%% Power", m_swingPower * 100.0f);
		RudeFontManager::GetFont(kDefaultFont)->Printf(160.0f, kPowerY, 0.0f, FONT_ALIGN_CENTER, kBallDistanceTopColor, kBallDistanceBotColor, "%.0f%% Power", m_swingPower * 100.0f);
		
		RudeFontManager::GetFont(kDefaultFontOutline)->Printf(160.0f, kAngleY, 0.0f, FONT_ALIGN_CENTER, kBallDistanceOutlineTopColor, kBallDistanceOutlineBotColor, "%.0f%% Angle", m_swingAngle * 100.0f);
		RudeFontManager::GetFont(kDefaultFont)->Printf(160.0f, kAngleY, 0.0f, FONT_ALIGN_CENTER, kBallDistanceTopColor, kBallDistanceBotColor, "%.0f%% Angle", m_swingAngle * 100.0f);
		
	}
	
	if(showClubInfo)
	{
		RBGolfClub *club = RBGolfClub::GetClub(m_curClub);
		char clubInfo[60];
		sprintf(clubInfo, "%.0f yds", club->m_dist);
		RudeFontManager::GetFont(kDefaultFontOutline)->Printf(10.0f, kClubDistY, 0.0f, FONT_ALIGN_LEFT, kBallDistanceOutlineTopColor, kBallDistanceOutlineBotColor, clubInfo);
		RudeFontManager::GetFont(kDefaultFont)->Printf(10.0f, kClubDistY, 0.0f, FONT_ALIGN_LEFT, kBallDistanceTopColor, kBallDistanceBotColor, clubInfo);
	}
	
	m_parText.SetValue(m_par);
	m_parText.Render();
	
	m_remainingDistText.SetValue(m_ballToHoleDist);
	m_remainingDistText.Render();
	
	m_strokeText.SetValue(m_stroke);
	m_strokeText.Render();
	
	m_scoreText.SetValue(m_score);
	m_scoreText.Render();

}

void RBTGame::Render(float aspect)
{
	RGL.SetViewport(0, 0, 480-1, 320-1);

	
	m_curCamera->SetView(aspect);
	RGL.LoadIdentity();
	
	
	
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
	
	RGL.Enable(kDepthTest, true);
	m_golfer.Render();
	
	
	//m_ballRecorder.RenderRecords();
	
	
	//RGL.Enable(kDepthTest, false);
	m_ball.Render();
	
	
	
	if(m_state == kStatePositionSwing2)
	{
		RGL.LoadIdentity();
		m_ballGuide.Render();
	}
	
	RGL.LoadIdentity();
	
	RenderCalcOrthoDrawPositions();
	
	
	RGL.Enable(kDepthTest, false);
	RGLD.RenderDebug();
	
	RGL.Ortho(0.0f, 0.0f, 0.0f, 320.0f, 480.0f, 100.0f);
	RGL.LoadIdentity();
	RGL.Enable(kBackfaceCull, false);
	RGL.Enable(kDepthTest, false);
	
	//RudeFontManager::GetFont(kDefaultFont)->Write(10.0f, 350.0f, 0.0f, "This is a TEST!", 0, FONT_ALIGN_LEFT, 0xFF000000, 0xFF0000FF);
	
	
	switch(m_state)
	{
		case kStatePositionSwing:
		case kStatePositionSwing2:
			RenderGuide(aspect);
			
			m_swingButton.Render();
			m_nextClubButton.Render();
			m_prevClubButton.Render();
			m_clubButton.Render();
			m_cameraButton.Render();
			RenderShotInfo(false, true);
			break;
		case kStateExecuteSwing:
			m_moveButton.Render();
			m_swingControl.Render();
			m_clubButton.Render();
			RenderShotInfo(false, true);
			break;
			
		case kStateFollowBall:
			//RenderBallFollowInfo(false);
			RenderShotInfo(true, false);
			break;
			
		case kStateRegardBall:
			//RenderBallFollowInfo(true);
			RenderShotInfo(true, false);
			
			//RudeFontManager::GetFont(kDefaultFont)->Printf(0.0f, 440.0f, 0.0f, FONT_ALIGN_LEFT, 0xFFFFFFFF, 0xFFFFFFFF, "POWER: %.0f%%", m_swingPower * 100.0f);
			
			break;
			
		case kStateBallInHole:
			
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
	
	switch(m_state)
	{
		case kStatePositionSwing:
		case kStatePositionSwing2:
			m_moveGuide = false;
			m_moveHeight = false;
			m_swingButton.TouchDown(rbt);
			m_swingCamAdjust.TouchDown(rbt);
			if(m_prevClubButton.TouchDown(rbt))
				NextClub(-1);
			if(m_nextClubButton.TouchDown(rbt))
				NextClub(1);
			if(m_cameraButton.TouchDown(rbt))
			{
				if(m_state == kStatePositionSwing)
					SetState(kStatePositionSwing2);
				else
					SetState(kStatePositionSwing);
			}
			break;
		case kStateExecuteSwing:
			m_swingControl.TouchDown(rbt);
			m_moveButton.TouchDown(rbt);
			break;
		case kStateFollowBall:
			m_swingButton.TouchDown(rbt);
			break;
	}
		
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
	
	switch(m_state)
	{
		case kStatePositionSwing:
		case kStatePositionSwing2:
			if(m_swingButton.TouchUp(rbt))
				SetState(kStateExecuteSwing);
			m_swingCamAdjust.TouchUp(rbt);
			m_nextClubButton.TouchUp(rbt);
			m_prevClubButton.TouchUp(rbt);
			break;
		case kStateExecuteSwing:
			if(m_swingControl.TouchUp(rbt))
			{
				if(m_swingControl.WillSwing())
					SetState(kStateWaitForSwing);
				
			}
			if(m_moveButton.TouchUp(rbt))
				SetState(kStatePositionSwing);
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
			SetState(kStateTeePosition);
			break;
	}
}

void RBTGame::Pause()
{

}


