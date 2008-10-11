
#ifndef _H_RudeSound
#define _H_RudeSound

#include "RudeGlobals.h"
#include "RudeDebug.h"

#ifdef RUDE_IPHONE
#define RUDE_SOUND_IPHONE
#include "MikSound.h"
#include <AudioToolbox/AudioToolbox.h>
#endif


#ifdef RUDE_SYMBIAN
#include <coemain.h>
#include "SoundStream.h"
#include "SoundProvider.h"
#define RUDE_SOUND_SYMBIAN

#define kRudeSoundNumSounds 45
#endif

enum {
	kSoundSwingWood = 0,
	kSoundSwingWedge,
	kSoundSwingWedgeInSand,
	kSoundSwingDriver,
	kSoundSwingIronSoft,
	kSoundSwingPutter,
	kSoundBallInHole,
};

#ifdef RUDE_SYMBIAN
static const TUid kRudeSoundUid = { KUidVectorBlaster+1 };
class RudeSound : public CCoeStatic
#else
class RudeSound
#endif
{
public:
#ifdef RUDE_SYMBIAN
	RudeSound(TUid uid);
#else
	RudeSound();
#endif
	~RudeSound();

public:

	static RudeSound * GetInstance();

	void PlaySong(char *song);

	void PlayWave(int num);

	void Tick();

	void Pause();
	void Unpause();

	void ToggleSound();

	void SoundOn(bool soundon) { _soundon = soundon; }
	bool SoundOn() { return _soundon; }

	void Shutdown();

private:
#ifdef RUDE_SOUND_SYMBIAN
	TPtr8* LoadFile(const TDesC& filename);
#endif
	void LoadWave(char *sound, int num);

	bool _soundon;

	//RudeKeyMap *_keymap;

#ifdef RUDE_SOUND_SYMBIAN
	RSoundStreamClient* iSoundStream;

	CModule* iModule;
	CSoundPlayer* iSoundPlayer;
	CSample **_samples;
#endif
	
#ifdef RUDE_SOUND_IPHONE
	
	SystemSoundID _soundids[20];
#endif

};

#endif