

#include "RudeSound.h"



#ifdef RUDE_WIN
#include <windows.h>
#endif

#ifdef RUDE_SYMBIAN
#include <bautils.h>
#include <eikappui.h> 
#include <eikapp.h>
#endif


#ifdef _WIN32_WCE
#ifndef _i386_
#ifndef SH3

#include "sound.h"
int gameon = 1;
#define RUDE_SOUND_DMIK

#endif
#endif
#endif

#ifdef RUDE_SYMBIAN
RudeSound::RudeSound(TUid uid)
: CCoeStatic(uid, EApp)
#else
RudeSound::RudeSound()
#endif
{
	
	_soundon = true;

#ifdef RUDE_SOUND_DMIK

	soundFreq = 2;
	soundStereo = 1;
	soundQuality = 3;

#ifdef WIN32_PLATFORM_WFSP
	soundStereo = 0;
	soundQuality = 2;
#endif

	soundInit();
	setMusicVolume(90);
	
	PlaySong("silence.it");

#endif
	
#ifdef RUDE_SOUND_IPHONE
	LoadWave("swing_wood.wav", kSoundSwingWood);
	
	LoadWave("swing_wedge.wav", kSoundSwingWedge);
	LoadWave("swing_wedge_insand.wav", kSoundSwingWedgeInSand);
	LoadWave("swing_driver.wav", kSoundSwingDriver);
	LoadWave("swing_iron_soft.wav", kSoundSwingIronSoft);
	LoadWave("swing_putter.wav", kSoundSwingPutter);
	LoadWave("ball_in_hole.wav", kSoundBallInHole);
#endif

#ifdef RUDE_SOUND_SYMBIAN
	// New Session to SoundServer
	iSoundStream = new RSoundStreamClient();
	iSoundStream->Connect();

	// Install MOD player as SoundProvider 
	iModule = new CModule();
	iSoundStream->AddSoundProvider(iModule);
	
	// Install sound player with 3 concurrent sounds
	iSoundPlayer = new CSoundPlayer(3);
	iSoundStream->AddSoundProvider(iSoundPlayer);

	// Sum of all volumes should not be greater than 256
	iModule->SetVolume(128);
	iSoundPlayer->SetVolume(128);
	
	_samples = (CSample **) malloc(kRudeSoundNumSounds * sizeof(CSample *));

	for(int i = 0; i < kRudeSoundNumSounds; i++)
		_samples[i] = NULL;
	
	LoadWave("vblaster.znd", kSoundVectorBlaster);
	LoadWave("fire.znd", kSoundFire);
	LoadWave("boom.znd", kSoundBoom);
	LoadWave("bigboom.znd", kSoundBigBoom);
	LoadWave("damage.znd", kSoundDamage);
	LoadWave("dead.znd", kSoundDead);
	LoadWave("powerup.znd", kSoundPowerUp);
	LoadWave("gameover.znd", kSoundGameOver);
	LoadWave("level.znd", kSoundLevelUp);
	
	iSoundStream->Open();
#endif


	
}

RudeSound::~RudeSound()
{
#ifdef RUDE_SOUND_DMIK
	soundShutdown();
#endif


}

void RudeSound::Shutdown()
{
#ifdef RUDE_SOUND_SYMBIAN
	// Make sure you close the stream, else your phone will crash!
	iSoundStream->Stop();
	iSoundStream->Close();

	delete iSoundStream;
	delete iSoundPlayer;
	delete iModule;
	
	for(int i = 0; i < kRudeSoundNumSounds; i++)
	{
		if(_samples[i] != NULL)
			delete _samples[i];
	}
	
	free(_samples);
#endif
}

#ifdef RUDE_SYMBIAN
RudeSound * RudeSound::GetInstance()
{
	CCoeEnv *coe = CCoeEnv::Static();
	return (RudeSound *) coe->Static(kRudeSoundUid);
}
#else
RudeSound *soundinstance = 0;

RudeSound * RudeSound::GetInstance()
{
	if(soundinstance == 0)
		soundinstance = new RudeSound();
	return soundinstance;
}
#endif


#ifdef RUDE_SOUND_SYMBIAN
TPtr8* RudeSound::LoadFile(const TDesC& filename)
{
	int size=0;
	RFs iRFs;
	RFile iFile;
	TPtr8* bufferDes;
	
	/*_debug->Print("LoadFile", 0);

	TFileName aPath=
		BaflUtils::DriveAndPathFromFullName(
			CEikonEnv::Static()->EikAppUi()->Application()->AppFullName());
	aPath.Append(filename);
	
	_debug->Print("LoadFile", 1);*/

	iRFs.Connect();

	int err = iFile.Open(iRFs, filename, EFileStream);
	
	if(err != KErrNone)
	{
		_debug->Print("LoadFile Open:", err);
		return NULL;
	}
	
	
	iFile.Size(size);

	bufferDes = new TPtr8(new TUint8[size], size);
	
	iFile.Read(*bufferDes, size);
	
	iFile.Close();
	iRFs.Close();

	return bufferDes;
}
#endif


void RudeSound::PlaySong(char *song)
{
	
#ifdef RUDE_SOUND_IPHONE
	MikSound_PlayModule(song);
#endif
	
	
#ifdef RUDE_SOUND_DMIK
	playSong(song);
#endif

#ifdef RUDE_SOUND_SYMBIAN

	if(!_soundon)
		return;

	char filename[150];
	RudeGlobals::GetPath(filename);
	strcat(filename, song);

	int tlen = strlen(filename) * 2;
	char *t = (char *) malloc(tlen);

	if(t == NULL)
		return;

	for(unsigned int i = 0; i < strlen(filename); i++)
	{
		t[i*2] = filename[i];
		t[i*2 + 1] = '\0';
	}

	TPtrC tptr((TUint16 *) t, strlen(filename));
	
	_debug->Print("PlaySong:LoadFrom", filename);
	TPtr8 *data = LoadFile(tptr);
	if(data == NULL)
		return;
	
	iModule->LoadFrom(data);
	
	
	_debug->Print("PlaySong:Start");
	iModule->Start();
	
	free(t);

#endif
}

void RudeSound::PlayWave(int num)
{
#ifdef RUDE_SOUND_DMIK
	playSound(num);
#endif
	
#ifdef RUDE_SOUND_IPHONE
	AudioServicesPlaySystemSound (_soundids[num]);
#endif

#ifdef RUDE_SOUND_SYMBIAN
	if(!_soundon)
		return;

	if(num >= kRudeSoundNumSounds)
		return;
		
	if(_samples[num] != NULL)
		iSoundPlayer->PlaySample(_samples[num]);
	
#endif
}


void RudeSound::LoadWave(char *sound, int num)
{
#ifdef RUDE_SOUND_DMIK
	loadSound(sound, num);
#endif
	
#ifdef RUDE_SOUND_IPHONE
	CFBundleRef bundle = CFBundleGetMainBundle();
	CFStringRef file = CFStringCreateWithCString(0, sound, kCFStringEncodingASCII);
	CFURLRef myURLRef = CFBundleCopyResourceURL(bundle, file, 0, 0);
	
	OSStatus error = AudioServicesCreateSystemSoundID (myURLRef, &_soundids[num]);

	
#endif

#ifdef RUDE_SOUND_SYMBIAN

	if(num >= kRudeSoundNumSounds)
		return;

	char filename[150];
	RudeGlobals::GetPath(filename);
	strcat(filename, sound);

	int tlen = strlen(filename) * 2;
	char *t = (char *) malloc(tlen);

	if(t == NULL)
		return;

	for(unsigned int i = 0; i < strlen(filename); i++)
	{
		t[i*2] = filename[i];
		t[i*2 + 1] = '\0';
	}

	TPtrC tptr((TUint16 *) t, strlen(filename));
	
	_debug->Print("LoadWave:new CSample", filename);
	_samples[num] = new CSample();
	_debug->Print("LoadWave:LoadFromZND");
	
	TPtr8 *data = LoadFile(tptr);
	if(data == NULL)
		return;
	
	_samples[num]->LoadFromZND(data);

	free(t);

	_debug->Print("LoadWave e");

#endif
}

void RudeSound::Tick()
{
#ifdef RUDE_SOUND_DMIK

	if(_soundon)
	{
		updateSoundBuffers();
		Sleep(0);
	}

#endif
}

void RudeSound::ToggleSound()
{
	if(_soundon)
	{
		_soundon = false;
		
#ifdef RUDE_SOUND_DMIK
		gameon = 0;
#endif
	}
	else
	{
		_soundon = true;
		
#ifdef RUDE_SOUND_DMIK
		gameon = 1;
#endif

	}

}

void RudeSound::Pause()
{
#ifdef RUDE_SOUND_DMIK
	gameon = 0;
#endif

}

void RudeSound::Unpause()
{
#ifdef RUDE_SOUND_DMIK
	if(_soundon)
		gameon = 1;
#endif
}