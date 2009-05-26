#ifndef __SOUNDPROVIDER_H__
#define __SOUNDPROVIDER_H__

#include "SoundStream.h"

// tables that are used to calculate note frequencies

// frequency*2^(finetune/8/12)
//Value:    0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F
//Finetune: 0  +1  +2  +3  +4  +5  +6  +7  -8  -7  -6  -5  -4  -3  -2  -1

const TUint MODfinetune[] = {65535, 66010, 66488, 66970, 67455, 67944, 68436, 68932,
	61856, 62305, 62757, 63211, 63669, 64131, 64959, 65064} ;

// frequency*2^(add/12)
const TUint MODtranspose[] = {65535, 69432, 73561, 77935, 82569, 87479, 92680, 98192,
	104030, 110216, 116770, 123714, 131070, 138864, 147121, 155869};


// Generic sample. Can be used for sound effects and music
class CSample {
public:
	CSample();
		
	// load sample with frequency attached (zSample)
	void LoadFromZND(TDesC8* inPtr);

	// load sample as raw sample (TInt8)
	void LoadFromRAW(TDesC8* inPtr, TUint16 srate);

public:
	// sample length
	TUint length;

	// start offset of loop
	TUint repStart;

	// length of loop
	TUint repLen;

	// Samplerate/frequency of the loaded sample
	TUint16 samplerate;

	// Pointer to sample data
	const TInt8* data;
};


class CChannel {

public:
	CChannel();

	TBool isActive();
	void SetOffset(TUint ofs);
	TBool SetSample(CSample* s);
	void Trigger(int freq);
	void SetFreq(int f) ;
	void SetVolume(int v);
	void Stop();

	void Calc(TInt16* buffer, int pos, int len);

public:
	CSample* sample;
	TUint samplepos;
	TUint freq;
	TUint volume;
	TUint step;
};

// MOD instrument
class CModuleInstrument : public CSample {
public:
	CModuleInstrument();

	// MOD finetune value
	TInt8 finetune;

	// default volume of the instrument
	TUint8 volume;
};

// MOD channel
class CModuleChannel : public CChannel {
public:
	// triggers sample with given period
	void TriggerPeriod(TUint period);

	// retriggers sample
	void ReTrigger();

	// updates internal effect tick
	inline void Tick();

	// return effect counter (used for arpeggio)
	inline TUint GetEffectCounter();

	// sets the period without stopping the sample
	void SetPeriod(TUint period);

	// adds the relPeriod to the set period
	void SetPeriodRelative(TInt relPeriod);

	// returns last period
	inline TUint GetPeriod();

	// transposes the sample by given halfnotes
	void TransposePeriod(TUint8 halfnotes);

	// slide to newperiod
	void SlideToPeriod(TUint newperiod, TUint speed);

protected:
	TUint GetPeriodFrequency();
	void UpdatePeriodFrequency();

	TUint effectCounter;
	TUint period;
};

// class that can play Amiga MODules
class CModule : public ASoundProvider {
public:
	CModule();

	virtual ~CModule();

	// initalizes the player with the given descriptior
	void LoadFrom(TDesC8* inPtr);
	void CalcBuffer(TInt16* buffer, TInt size);

	// starts playing
	void Start();

	// stops playing
	void Stop();

private:
	void SetBPM(int bpm);
	void UpdateTick();

//public:
	// current position in the pattern sequence
	TUint8 currentSeq;

	// current row
	TInt8 currentRow;

	// all used instruments (always 31)
	CModuleInstrument* samples;

	// all used channels (count varies)
	CModuleChannel* channels;
	// channel count
	TUint8 channelnum;
	
	// length of the song in patterns
	TUint8 songlength;
	// pointer to pattern sequence
	const TUint8 *patternSeq;

	// number of different pattern
	TUint8 patternNum;
	// pointer to first pattern
	const TUint8 *patterndata;

	// ticks (updates) per row
	TInt ticksPerRow;

	// tick length in samples
	TInt ticklen;

	// number of current tick
	TInt currentTick;

	// how many samples of the current tick are left?
	TUint tickSamplesLeft;
	
	// are we playing
	TBool playing;
};

// sample player which may be used for sound effects
class CSoundPlayer : public ASoundProvider {
public:
	// channelnum is the count of simultaneous active samples
	CSoundPlayer(TUint8 channelnum);

	// virtual method that is used by the ASoundProvider interface
	virtual void CalcBuffer(TInt16 *data, TInt size);
	
	// plays the given sample. If no channel is free the longest playing
	// sample is stopped
	void PlaySample(CSample* sample);
	
	virtual ~CSoundPlayer();

private:
	int channelnum;
	CChannel* channels;
	int* playdur;
};

#endif