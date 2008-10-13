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
	
	m_swingControl.SetRect(RudeRect(0,0,400,320));
	m_swingControl.SetGolfer(&m_golfer);
	
	m_swingButton.SetRect(RudeRect(480 - 60, 180, 480, 320));
	m_swingButton.SetTextures("swing", "swing");
	
	m_moveButton.SetRect(RudeRect(480 - 60, 180, 480, 320));
	m_moveButton.SetTextures("move", "move");
	
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
	btVector3 ball = m_ball.GetPosition();
	
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
	
	m_ballRecorder.NextFrame(delta, true);
	
	
	btVector3 dist = ball - m_dBall;
	dist.setY(0.0f);
	float df = dist.length();
	m_ballShotDist = df / 3.0f;
	
	btVector3 ballToHole = m_terrain.GetHole() - ball;
	ballToHole.setY(0.0f);
	m_ballToHoleDist = ballToHole.length() / 3.0f;
	
	
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
	m_dGuide = newGuide;
}

void RBTGame::HitBall()
{
	
	m_swingPower = m_swingControl.GetPower();
	m_swingAngle = m_swingControl.GetAngle();
	
	
	RBGolfClub *club = RBGolfClub::GetClub(m_curClub);
	
	RudeSound::GetInstance()->PlayWave(club->m_swingsound);
	
	
	
	float loft = club->m_loft;
	loft = (loft / 180.0f) * 3.1415926f;
	
	//RudeScreenVertex downStroke, upStroke;
	//float downTime, upTime;
	//m_swingControl.GetStroke(downStroke, downTime, upStroke, upTime);
	
	
	
	btVector3 ball = m_ball.GetPosition();
	btVector3 guide = m_terrain.GetGuidePoint(ball);
	btVector3 aimvec = guide - ball;
	aimvec.setY(0);
	aimvec.normalize();
	
	btMatrix3x3 guidemat;
	guidemat.setEulerYPR(m_swingYaw, 0.0f, 0.0f);
	aimvec = guidemat * aimvec;
	
	aimvec.setY(tan(loft));
	aimvec.normalize();
	
	
	
	
	
	//btMatrix3x3 loftmat;
	//loftmat.setEulerYPR(0.0f, 0.0f, 0.0f);
	//btVector3 trajectory = loftmat * aimvec;
	//trajectory.normalize();
	
	m_ball.SetForce(aimvec * club->m_power * m_swingPower);
	//m_ball.SetForce(btVector3(-1000,0,-1000));
	
	
	
}



void RBTGame::NextFrame(float delta)
{
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

	RGLD.DebugDrawLine(m_dGuide, m_dGuide + btVector3(0,10,0));
	
	m_golfer.NextFrame(delta);
	
	
	m_ball.NextFrame(delta);
	m_curCamera->NextFrame(delta);
	
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
	
	RudeFontManager::GetFont(kDefaultFontOutline)->Printf(10.0f, kParY, 0.0f, FONT_ALIGN_LEFT, kParOutlineTopColor, kParOutlineBotColor, "PAR %d", m_par);
	RudeFontManager::GetFont(kDefaultFont)->Printf(10.0f, kParY, 0.0f, FONT_ALIGN_LEFT, kParTopColor, kParBotColor, "PAR %d", m_par);
	
	RudeFontManager::GetFont(kDefaultFontOutline)->Printf(10.0f, kRemainDistY, 0.0f, FONT_ALIGN_LEFT, kBallRemainingOutlineTopColor, kBallRemainingOutlineBotColor, "%.0f yds", m_ballToHoleDist);
	RudeFontManager::GetFont(kDefaultFont)->Printf(10.0f, kRemainDistY, 0.0f, FONT_ALIGN_LEFT, kBallRemainingTopColor, kBallRemainingBotColor, "%.0f yds", m_ballToHoleDist);
	
	
	char score[60];
	if(m_score == 0)
		sprintf(score, "±0");
	else
		sprintf(score, "%d", m_score);
	
	RudeFontManager::GetFont(kDefaultFontOutline)->Printf(310.0f, kParY, 0.0f, FONT_ALIGN_RIGHT, kParOutlineTopColor, kParOutlineBotColor, "Stroke %d", m_stroke);
	RudeFontManager::GetFont(kDefaultFont)->Printf(310.0f, kParY, 0.0f, FONT_ALIGN_RIGHT, kParTopColor, kParBotColor, "Stroke %d", m_stroke);
	
	RudeFontManager::GetFont(kDefaultFontOutline)->Printf(310.0f, kRemainDistY, 0.0f, FONT_ALIGN_RIGHT, kBallRemainingOutlineTopColor, kBallRemainingOutlineBotColor, score);
	RudeFontManager::GetFont(kDefaultFont)->Printf(310.0f, kRemainDistY, 0.0f, FONT_ALIGN_RIGHT, kBallRemainingTopColor, kBallRemainingBotColor, score);
	
}

void RBTGame::Render(float aspect)
{
	RGL.SetViewport(0, 0, 480-1, 320-1);
	
	if(gDebugCamera)
		m_curCamera = &m_debugCamera;
	else
		m_curCamera = &m_ballCamera;
	
	m_curCamera->SetView(aspect);
	RGL.LoadIdentity();
	
	
	
	m_skybox.Render();
	
	RGL.Enable(kDepthTest, true);
	m_terrain.Render();
	
	if(m_state == kStateFollowBall)
	{
		m_ballRecorder.RenderTracers();
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
	m_ball.SetForce(lookdir * 1000);
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
					SetState(kStateHitBall);
				
			}
			if(m_moveButton.TouchUp(rbt))
				SetState(kStatePositionSwing);
			break;
		case kStateFollowBall:
			if(m_swingButton.TouchUp(rbt))
				SetState(kStateTeePosition);
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


