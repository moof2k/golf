#ifndef __SOUNDSTREAM_H__
#define __SOUNDSTREAM_H__

#include <mdaaudiooutputstream.h>
#include <mda\common\audio.h>
#include <eikenv.h>

#define kDefaultMessageSlots 4

#ifdef __WINS__
static const TInt KSoundFPS = 10;
#else
static const TInt KSoundFPS = 64;
#endif

static const TUint SAMPLERATE = 16000;
static const TInt KSoundInterval = 1000000/KSoundFPS;

static const TInt SOUNDBUFSIZE =	SAMPLERATE/KSoundFPS;
static const TInt SOUNDBUFMAXSIZE =	SOUNDBUFSIZE*2;


_LIT(KSoundStreamServerName,"SoundStreamServer");
_LIT(KSoundStreamServerSemaphoreName, "SoundStreamServerSemaphore");

enum TSoundStreamServerPanic {
    EBadRequest = 1,
    EBadDescriptor = 2,
    ESrvCreateServer = 3,
    EMainSchedulerError = 4,
    ECreateTrapCleanup = 5,
    ESrvSessCreateTimer = 6,
    EReqAlreadyPending = 7
   };


enum TSoundStreamServRqst {
	ESoundStreamServAddSoundProvider,
	ESoundStreamServSetPaused,
	ESoundStreamServStop,
	ESoundStreamServOpen
};

// abstract base class for our sound providers
class ASoundProvider {
public:
	ASoundProvider();

	virtual void CalcBuffer(TInt16 *data, TInt size) = NULL;
	virtual void SetVolume(TUint8 volume);

	TUint8 iVolume;
};


// thread function for server
GLREF_C TInt CSoundStreamServerThread(TAny *);


class CSoundStreamServer : public CServer {
public:
	enum {EPriority=950}; // mpt - need to explain the magic here!

public:
	static CSoundStreamServer* NewLC();
	virtual CSharableSession *NewSessionL(const TVersion &/*aVersion*/) const;
	static TInt ThreadFunction(TAny* aStarted);

protected:
	CSoundStreamServer(TInt aPriority);
	static void ThreadFunctionL();
	static void PanicServer(TSoundStreamServerPanic aPanic);

private:
	TInt iActive;
};

class CSoundStreamServerSession : public MMdaAudioOutputStreamCallback, public CSession {
public:	
	//CSoundStreamServerSession();
	virtual ~CSoundStreamServerSession();
	
	CSoundStreamServerSession(RThread &aClient, CSoundStreamServer* aServer);
	static CSoundStreamServerSession* NewL(RThread &aClient, CSoundStreamServer* aServer);
	
	//service request
	virtual void ServiceL(const RMessage &aMessage);
	void DispatchMessageL(const RMessage &aMessage);

	void SetPaused(TBool paused);
    
	// from MMdaAudioOutputStreamCallback
	virtual void MaoscOpenComplete(TInt aError);
	virtual void MaoscBufferCopied(TInt aError, const TDesC8& aBuffer);
	virtual void MaoscPlayComplete(TInt aError);
	
	void AddSoundProvider(ASoundProvider* a) {
		iSoundProviders.Append(a);
	}

private:
	// safewrite between client and server
	void Write(const TAny* aPtr,const TDesC8& aDes,TInt anOffset=0);

	void UpdateBuffer();
	void Open();

	void ReOpen();

	CMdaAudioOutputStream* iStream;
	TMdaAudioDataSettings iSettings;

	TInt16 iSoundData[SOUNDBUFMAXSIZE];
	TPtr8* iSoundBuf;
	
	RArray<ASoundProvider*> iSoundProviders;
	TBool initialized;
	TBool readyForWrite;
	TUint lastUpdateTick;
	TUint bufferPosition;
	TTime timeOfUpdate;

	CSoundStreamServer *iServer;

	TBool iPaused;
};

// client side interface to soundstream
class RSoundStreamClient : public RSessionBase {
public:
	RSoundStreamClient();
	TInt Connect();
	
	int AddSoundProvider(ASoundProvider* a);
	int Open();
	int Stop();
	int SetPaused(TBool paused);
};

#endif