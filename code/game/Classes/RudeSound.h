
#ifndef _H_RudeSound
#define _H_RudeSound

#include "Rude.h"
#include "RudeGlobals.h"
#include "RudeDebug.h"

#include "MikSound.h"

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
	kSoundUIStart,
	kSoundUIBack,
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

private:

	void LoadWave(const char *sound, eSoundEffect num);

	bool m_soundon;
	
	UInt32 m_soundids[kNumSounds];
	eSoundBGM m_curBGM;
	
	float m_bgmVol;
	float m_bgmVolFade;
	bool m_bgmVolFadeEnabled;

};

#endif