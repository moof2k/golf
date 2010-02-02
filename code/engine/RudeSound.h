/*
 *  RudeSound.h
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#ifndef _H_RudeSound
#define _H_RudeSound

#include "Rude.h"
#include "RudeGlobals.h"
#include "RudeDebug.h"

#include "SoundEngine.h"

typedef enum {
	kSoundNone = -1,
	kSoundSwingWood = 0,
	kSoundSwingWedge,
	kSoundSwingWedgeInSand,
	kSoundSwingDriver,
	kSoundSwingIronSoft,
	kSoundSwingPutter,
	kSoundBallInHole,
	kSoundMissedPutt,
	kSoundCheer,
	kSoundClaps,
	kSoundUIStart,
	kSoundUIBack,
	kSoundUIExit,
	kSoundUISelect,
	kSoundUIClickLow,
	kSoundUIClickHi,
	kNumSounds,
} eSoundEffect;

typedef enum {
	kBGMNone = -1,
	kBGMSilence,
	kBGMTitle,
	kBGMPutting,
	
	kNumBGMs,
} eSoundBGM;

class RudeSound
{
public:
	RudeSound();
	~RudeSound();

public:

	static RudeSound * GetInstance();

	void PlaySong(eSoundBGM song);
	void StopSong();
	
	bool ToggleMusic();

	void PlayWave(eSoundEffect num);

	void Tick(float delta);
	
	void BgmVolFade(float amount);
	void BgmVol(float vol);

	void Pause();
	void Unpause();

	void ToggleSound();

	void SoundOn(bool soundon) { m_soundon = soundon; }
	bool SoundOn() { return m_soundon; }

	void Shutdown();
	
	bool GetMusicOn() { return m_musicOn; }

private:

	void LoadWave(const char *sound, eSoundEffect num);

	bool m_soundon;
	bool m_musicOn;
	
#if defined(RUDE_IPHONE) || defined(RUDE_MACOS)
	SystemSoundID m_soundids[kNumSounds];
#endif

#if defined(RUDE_WIN)
	int m_soundids[kNumSounds];
#endif
	
	eSoundBGM m_curBGM;
	
	float m_bgmVol;
	float m_bgmVolFade;
	bool m_bgmVolFadeEnabled;

};

#endif