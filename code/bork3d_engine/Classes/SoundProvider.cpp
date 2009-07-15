#include "soundprovider.h"

CSample::CSample() {
	length=0;
	data=NULL;
}

// load sample from ZND (automatically sets sample rate)
void CSample::LoadFromZND(TDesC8* inPtr) {
	repStart=repLen=0;
	length=inPtr->Length() - 2;
	samplerate=*((TUint16*)inPtr->Ptr());
	data=(TInt8*) (inPtr->Ptr()+2);
}

// load RAW sample, sample rate set manually
void CSample::LoadFromRAW(TDesC8* inPtr, TUint16 srate) {
	repStart=repLen=0;
	length=inPtr->Length();
	samplerate=srate;
	data=(TInt8*) (inPtr->Ptr());
}

CModule::CModule() {
	playing = false;
	channels = NULL;
	samples = new CModuleInstrument[31];
}

CModule::~CModule() {
	delete samples;
	delete channels;
}

void CModule::Start() {
	playing = true;
}

void CModule::Stop() {
	playing = false;
}

// load module, initialize needed data structures
void CModule::LoadFrom(TDesC8* inPtr) {
	int i,ofs;

	playing = false;
	if (channels != NULL) {
		delete channels;
	}

	const TUint8* in=inPtr->Ptr();
	
	for (i=0; i<31; i++) {
		samples[i].finetune=in[44+i*30];

		samples[i].length=((in[42+i*30]<<8)+in[42+i*30+1])*2;
		samples[i].volume=in[45+i*30];
		samples[i].repStart=((in[46+i*30]<<8)+in[46+i*30+1])*2;
		samples[i].repLen=((in[48+i*30]<<8)+in[48+i*30+1])*2;
		if (samples[i].repLen<8) {
			samples[i].repLen=0;
		}
	}

	songlength=in[950];
	patternSeq=&in[952];

	patternNum=0;

	for (i=0; i<128; i++) {
		if (patternSeq[i]>patternNum)
			patternNum=patternSeq[i];
	}

	if ((in[1082]=='C') && (in[1083]=='H')) {
		channelnum=(TUint8)((in[1080]-'0')*10+(in[1081]-'0'));
	} else if (in[1081]=='C') {
		channelnum=(TUint8)(in[1080]-'0');
	} else {
		channelnum=4;
	}

	patterndata=&in[1084];

	ofs=1084+(patternNum+1)*channelnum*4*64;
	for (i=0; i<31; i++) {
		samples[i].data=(TInt8*) &in[ofs];
		ofs+=samples[i].length;
	}

	channels = new CModuleChannel[channelnum];

	currentSeq=0;
	currentRow=0;

	SetBPM(125);
	ticksPerRow=6;
	currentTick=-1;
	tickSamplesLeft=0;
	UpdateTick();
}

// set song speed
void CModule::SetBPM(int bpm) {
	ticklen=((1773447*1200)/709378*SAMPLERATE)/(bpm*1200);
}

void CModule::CalcBuffer(TInt16* buffer, TInt size) {	
	if (!playing) return;

	int ofs=0;
	int len, i;

	// render incomplete tick
	if (tickSamplesLeft>0) {
		len=Min((TInt)tickSamplesLeft, (TInt)size-ofs);
		for (i=0; i<channelnum; i++) {
			channels[i].Calc(buffer, ofs, len);
		}
		ofs+=len;
		tickSamplesLeft-=len;
	}

	// render as many ticks as possible
	while (ofs<size) {
		UpdateTick();
		if (size-ofs<(int)ticklen) {
			len=size-ofs;
			tickSamplesLeft=ticklen-len;
		} else {
			len=ticklen;
		}
		for (i=0; i<channelnum; i++) {
			channels[i].Calc(buffer, ofs, len);
		}
		ofs+=len;
	}
}

// perform all necessary operations to render next tick
void CModule::UpdateTick() {
	currentTick++;

	// is row update necessary?
	if (currentTick>=ticksPerRow) {
		currentTick=0;
		currentRow++;

		// is sequence update necessary?
		if (currentRow>=64) {
			currentRow=0;
			currentSeq++;
		
			// end of song? start over
			if (currentSeq>=songlength) {
				currentSeq=0;
			}
		}
	}

	// calculate current row offset
	int ofs=currentRow*4*channelnum+patternSeq[currentSeq]*4*channelnum*64;
	
	for (int i=0; i<channelnum; i++) {
		// get data for current note
		TUint8 sampleid=(TUint8)((patterndata[i*4+ofs]&0xf0) | (patterndata[i*4+ofs+2]>>4));
		TUint period=((patterndata[i*4+ofs]&0x0f)<<8) | patterndata[i*4+ofs+1];
		TUint8 effectcommand=(TUint8)(patterndata[i*4+ofs+2]&0x0f);
		TUint8 effectdata=(TUint8)(patterndata[i*4+ofs+3]);
		TUint8 extendedData = (TUint8)(effectdata & 0x0f);

		// beginning of row?
		if (currentTick==0) {
			// if a new sample is set...
			if (sampleid>0) {
				channels[i].SetSample(&samples[sampleid-1]);
			}

			// if note data is set
			if (period>0) {
				if (effectcommand != 0x3) { // != slide to note
					// trigger sample now except there is the command "sample delay"
					if ((effectcommand != 0xe) || ((effectdata & 0xf0) != 0xd0)) {
						channels[i].SetVolume(((CModuleInstrument*)channels[i].sample)->volume*4*iVolume/channelnum/255);
						channels[i].TriggerPeriod(period);
					}
				}
			}
		}

		// process effects
		switch (effectcommand) {
			case 0x0: // Arpeggio
				if (effectdata != 0) {
					// frequency=frequency*2^(add/12)
					switch (channels[i].GetEffectCounter() % 3) {
						case 0: // Original Period
							channels[i].TransposePeriod(0);
							break;
						case 1: // Period - xxxx halftones 
							channels[i].TransposePeriod((TUint8)(effectdata>>4));
							break;
						case 2: // Period - yyyy halftones
							channels[i].TransposePeriod((TUint8)(effectdata & 0x0f));
							break;
					}
				}
				break;
			case 0x1: // slide up
				if (currentTick>0) {
					channels[i].SetPeriodRelative(-effectdata);
				}
				break;
			case 0x2: // slide down
				if (currentTick>0) {
					channels[i].SetPeriodRelative(effectdata);
				}
				break;
			case 0x3: // slide to note
				if (period!=0) {
					channels[i].SlideToPeriod(period, effectdata);
				}
				break;
			case 0x9: // sample offset
				if (currentTick == 0) {
					channels[i].SetOffset(effectdata*256);
				}
				break;
			case 0xb: // position jump
				// do at last tick...
				if (currentTick>=ticksPerRow-1) {
					currentRow=0;
					currentSeq++;

					// end of song? start over
					if (currentSeq>=songlength) {
						currentSeq=0;
					}
				}
				break;
			case 0xc: // set volume
				// only at first tick
				if (currentTick==0) {
					channels[i].SetVolume(effectdata*4*iVolume/channelnum/255);
				}
				break;
			case 0xd: // pattern break
				//  do at last tick...
				if (currentTick>=ticksPerRow-1) {
					currentRow=(TUint8)(effectdata-1);
					currentSeq++;

					// end of song? start over
					if (currentSeq>=songlength) {
						currentSeq=0;
					}
				}
				break;
			case 0xe: // extended effects
				switch (effectdata & 0xf0) {
					case 0x50: // set finetune of current instrument
						if (currentTick==0) {
							((CModuleInstrument*)channels[i].sample)->finetune = extendedData;
						}
						break;
					case 0x90: // Retrigger
					case 0xd0: // Delay sample
						if (currentTick==extendedData) {
							channels[i].TriggerPeriod(period);
						}
						break;
					case 0xc0: // sample break
						if (currentTick==extendedData) {
							channels[i].Stop();
						}
						break;
					default:
						break;
				}
				break;
			case 0xf: // set speed / bpm
				if (currentTick==0) {
					if (effectdata<32) {
						// set ticks per row
						ticksPerRow=effectdata;	
					} else {
						// set BPM
						SetBPM(effectdata);					
					}
				}
				break;
			default:
				break;
		}
		// update channel
		channels[i].Tick();
	}
}


void CChannel::Calc(TInt16* buffer, int pos, int len) {
	if (!isActive()) return;

	buffer+=pos;

	// do we have to check for sample end?
	if (samplepos+len*step < (sample->length << 8)) {
		for (int i = 0; i < len; i++) {
			// render sample
			buffer[i] = (TInt16)(buffer[i]+sample->data[samplepos >> 8] * volume);
			samplepos += step;
		}
	} else {
		// TODO: Optimize here
		for (int i = 0; i < len; i++) {

			// will we reach the end of the sample?
			if (samplepos >= sample->length << 8) {

				// is this a looped sample?
				if (sample->repLen>0) {
					// don't set directly to beginning of loop as clicks will occur
					samplepos -= (sample->repLen << 8);
				} else {
					// stop channel
					sample = NULL;
					return;
				}
			}

			// render sample
			buffer[i] = (TInt16)(buffer[i] + sample->data[samplepos >> 8] * volume);
			samplepos += step;
		}
	}
}

// is a sample being played?
TBool CChannel::isActive() {
	return ((sample!=NULL) && (samplepos < (sample->length << 8)));
}

CChannel::CChannel() {
	sample=NULL;
	volume=128;
}

// set sample offset
void CChannel::SetOffset(TUint ofs) {
	samplepos=ofs*0x100;
}

// set sample
TBool CChannel::SetSample(CSample* s) {
	if (s != sample) {
		sample=s;
		samplepos=0;
		return true;
	}
	return false;
}

// retrigger sample with specified sample rate
void CChannel::Trigger(int freq) {
	samplepos=0;
	SetFreq(freq);
}

// set sample rate
void CChannel::SetFreq(int f) {
	freq=f;
    step=freq*0x100/SAMPLERATE;
}

// set volume
void CChannel::SetVolume(int v) {
	volume=v;
}

// stop sample
void CChannel::Stop() {
	samplepos=0xffffff;
}

// initalize sound player with specified channel number
CSoundPlayer::CSoundPlayer(TUint8 channelnum) {
	this->channelnum=channelnum;
	channels = new CChannel[channelnum];
	playdur = new int[channelnum];
	for (int i=0; i<channelnum; i++) {
		playdur[i]=0xffffff;
	}
}

// render buffer
void CSoundPlayer::CalcBuffer(TInt16 *data, TInt size) {
	for (int i=0; i<channelnum; i++) {
		channels[i].Calc(data, 0, size);
		if (channels[i].isActive()) {
			playdur[i]++;
		} else {
			playdur[i]=0xffffff;
		}
	}	
}
	
CSoundPlayer::~CSoundPlayer() {
	delete channels;
	delete playdur;
}

// play specified sample
void CSoundPlayer::PlaySample(CSample* sample) {
	int maxdur=0;
	int maxchan=0;
	
	// search channel with longest play time
	for (int i=0; i<channelnum; i++) {
		if (playdur[i]>maxdur) {
			maxdur=playdur[i];
			maxchan=i;
		}
	}

	// start sample
	playdur[maxchan]=0;
	channels[maxchan].SetSample(sample);
	channels[maxchan].SetVolume(iVolume/channelnum);
	channels[maxchan].Trigger(sample->samplerate);
}

CModuleInstrument::CModuleInstrument() {
	volume=64; // standard MOD volume
}

// trigger sample with specified MOD period (sample rate)
void CModuleChannel::TriggerPeriod(TUint period) {
	SetPeriod(period);
	ReTrigger();
}

// retrigger sample
void CModuleChannel::ReTrigger() {
	Trigger(GetPeriodFrequency());
	effectCounter=0;
}

// update effect counter
inline void CModuleChannel::Tick() {
	effectCounter++;
}

inline TUint CModuleChannel::GetEffectCounter() {
	return effectCounter;
}

// set sample rate (MOD period)
void CModuleChannel::SetPeriod(TUint period) {
	this->period = period;
	UpdatePeriodFrequency();
}

// special functions used for slides
void CModuleChannel::SetPeriodRelative(TInt relPeriod) {
	SetPeriod(period+relPeriod);
}

inline TUint CModuleChannel::GetPeriod() {
	return period;
}

// special function used for arpeggio
void CModuleChannel::TransposePeriod(TUint8 halfnotes) {
	SetFreq(3546895/(period*0xffff/MODtranspose[halfnotes]));
}

// special function used for slides
void CModuleChannel::SlideToPeriod(TUint newperiod, TUint speed) {
	if (newperiod>period) {
		SetPeriod(period+speed);
	} else if (newperiod<period) {
		SetPeriod(period-speed);
	}
}

// formula to convert an Amiga period to sample rate
TUint CModuleChannel::GetPeriodFrequency() {
	return 3546895/(period*0xffff/
		MODfinetune[((CModuleInstrument*)sample)->finetune]);
}

void CModuleChannel::UpdatePeriodFrequency() {
	SetFreq(GetPeriodFrequency());
}
