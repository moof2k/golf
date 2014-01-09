/*
 *  RudeSound.cpp
 *
 *  Bork3D Game Engine
 *  Copyright (c) 2009 Bork 3D LLC. All rights reserved.
 *
 */

#include "RudeSound.h"

#include "RudeFile.h"
#include "RudeRegistry.h"

#define kListenerDistance			1.0

const char * kSoundFilenames[kNumSounds] = {
	"swing_wood.wav",
	"swing_wedge.wav",
	"swing_wedge_insand.wav",
	"swing_driver.wav",
	"swing_iron_soft.wav",
	"swing_putter.wav",
	"ball_in_hole.wav",
	"crowd_missedputt.wav",
	"sfx_cheer.wav",
	"sfx_claps.wav",
	"sfx_start.wav",
	"sfx_back.wav",
	"sfx_exit.wav",
	"sfx_select.wav",
	"sfx_clicklow.wav",
	"sfx_clickhi.wav",
};

const char * kSoundBGMs[kNumBGMs] = {
	"bgm_silence.m4a",
	"bgm_title.m4a",
	"bgm_putting.m4a"
};

RudeSound::RudeSound()
{
	m_soundon = true;
	m_musicOn = true;
	
	m_curBGM = kBGMNone;
	
	m_bgmVolFadeEnabled = false;
	m_bgmVol = 1.0;
	m_bgmVolFade = 0.0;

#if defined(RUDE_IPHONE) || defined(RUDE_MACOS)
	SoundEngine_Initialize(44100);
	SoundEngine_SetListenerPosition(0.0, 0.0, kListenerDistance);
#endif

	for(int i = 0; i < kNumSounds; i++)
	{
		m_soundids[i] = -1;

		LoadWave(kSoundFilenames[i], (eSoundEffect) i);
	}
	
	RudeRegistry *reg = RudeRegistry::GetSingleton();
	
	int loadsize = sizeof(m_musicOn);
	if(reg->QueryByte("GOLF", "RS_MUSIC", &m_musicOn, &loadsize) == 0)
	{
		
	}
	else
	{
		m_musicOn = true;
	}
	
}

RudeSound::~RudeSound()
{


}

void RudeSound::Shutdown()
{

}

RudeSound *soundinstance = 0;

RudeSound * RudeSound::GetInstance()
{
	if(soundinstance == 0)
		soundinstance = new RudeSound();
	return soundinstance;
}

void RudeSound::BgmVolFade(float amount)
{
	m_bgmVolFade = amount;
	m_bgmVolFadeEnabled = true;
}

void RudeSound::BgmVol(float vol)
{
	m_bgmVol = vol;
	m_bgmVolFade = 0.0f;
	
#if defined(RUDE_IPHONE) || defined(RUDE_MACOS)
	if(m_curBGM != kBGMNone)
		SoundEngine_SetBackgroundMusicVolume(m_bgmVol);
#endif
}

void RudeSound::PlaySong(eSoundBGM num)
{
	RUDE_REPORT("RudeSound::PlaySong %d\n", num);
	
	if(num == m_curBGM)
	{
		RUDE_REPORT("BGM already playing\n");
		return;
	}
	
	if(m_curBGM != kBGMNone)
	{
		RUDE_REPORT("Unloading previous BGM\n");

#if defined(RUDE_IPHONE) || defined(RUDE_MACOS)
		SoundEngine_UnloadBackgroundMusicTrack();
#endif
	}
	
	if(m_musicOn)
	{
		m_curBGM = num;
		
		char buffer[512];
		RudeFileGetFile(kSoundBGMs[m_curBGM], buffer, 512);
				
#if defined(RUDE_IPHONE) || defined(RUDE_MACOS)
		SoundEngine_LoadBackgroundMusicTrack(buffer, false, false);
		SoundEngine_StartBackgroundMusic();
#endif
	}
}

void RudeSound::StopSong()
{
	if(m_curBGM == kBGMNone)
		return;
	
	RUDE_REPORT("RudeSound::StopSong %d\n", m_curBGM);
	
	m_curBGM = kBGMNone;

#if defined(RUDE_IPHONE) || defined(RUDE_MACOS)
	SoundEngine_UnloadBackgroundMusicTrack();
#endif
}

bool RudeSound::ToggleMusic()
{
	m_musicOn = !m_musicOn;
	
	if(!m_musicOn)
	{
		StopSong();
	}
	
	RudeRegistry *reg = RudeRegistry::GetSingleton();
	reg->SetByte("GOLF", "RS_MUSIC", &m_musicOn, sizeof(m_musicOn));
	
	return m_musicOn;
}

void RudeSound::PlayWave(eSoundEffect num)
{
	if(num == kSoundNone)
		return;
	
#if 0
	int result = SoundEngine_StartEffect(m_soundids[num]);
	//RUDE_ASSERT(result == noErr, "Could not play effect (result = %d)\n", result);
#endif
	
#if defined(RUDE_IPHONE) || defined(RUDE_MACOS)
	AudioServicesPlaySystemSound(m_soundids[num]);
#endif

}


void RudeSound::LoadWave(const char *sound, eSoundEffect num)
{
	RUDE_ASSERT(num < kNumSounds, "Invalid sound num");
	RUDE_ASSERT(sound, "No sound name");
	RUDE_REPORT("RudeSound::LoadWave %s\n", sound);
	
#if 0
	char buffer[512];
	RudeFileGetFile(sound, buffer, 512);

	int result = SoundEngine_LoadEffect(buffer, &m_soundids[num]);
	//RUDE_ASSERT(result == noErr, "Could not load effect (result = %d)\n", result);
#endif
	
#if defined(RUDE_IPHONE) || defined(RUDE_MACOS)
	CFBundleRef bundle = CFBundleGetMainBundle();
	CFStringRef file = CFStringCreateWithCString(0, sound, kCFStringEncodingASCII);
	CFURLRef myURLRef = CFBundleCopyResourceURL(bundle, file, 0, 0);
	
	OSStatus error = AudioServicesCreateSystemSoundID(myURLRef, &m_soundids[num]);
	RUDE_ASSERT(error == kAudioServicesNoError, "Could not load sound %s", sound);
#endif
	
}

void RudeSound::Tick(float delta)
{
	if(m_bgmVolFadeEnabled && m_curBGM != kBGMNone)
	{
		m_bgmVol += m_bgmVolFade * delta;
		
		if(m_bgmVol > 1.0f)
		{
			m_bgmVol = 1.0f;
			m_bgmVolFade = 0.0f;
			m_bgmVolFadeEnabled = false;
		}
		else if(m_bgmVol < 0.0f)
		{
			m_bgmVol = 0.0f;
			m_bgmVolFade = 0.0f;
			m_bgmVolFadeEnabled = false;
			
			StopSong();
			return;
		}
		
#if defined(RUDE_IPHONE) || defined(RUDE_MACOS)
		SoundEngine_SetBackgroundMusicVolume(m_bgmVol);
#endif
		
	}
}

void RudeSound::ToggleSound()
{
	if(m_soundon)
	{
		m_soundon = false;
	}
	else
	{
		m_soundon = true;
	}

}

void RudeSound::Pause()
{

}

void RudeSound::Unpause()
{

}