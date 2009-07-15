#include "soundstream.h"
#include <bassnd.h>

#include "RudeDebug.h"

// ----------------------------------- ASoundProvider --------------------------------------
ASoundProvider::ASoundProvider() {
	iVolume=64;
}

void ASoundProvider::SetVolume(TUint8 volume) {
	this->iVolume=volume;
}

// ----------------------------------- CSoundStreamServer ----------------------------------

// Create and start a new count server.
CSoundStreamServer* CSoundStreamServer::NewLC() {
	CSoundStreamServer *pS=new (ELeave) CSoundStreamServer(EPriority);
	pS->StartL(KSoundStreamServerName);
    CleanupStack::PushL(pS) ;
    return pS;
}

// Create a new server session.
CSharableSession *CSoundStreamServer::NewSessionL(const TVersion &/*aVersion*/) const {
	RThread aClient = Message().Client();
	return CSoundStreamServerSession::NewL(aClient, (CSoundStreamServer*)this);
}

CSoundStreamServer::CSoundStreamServer(TInt aPriority)
	: CServer(aPriority) {
}

// The server thread.
void CSoundStreamServer::ThreadFunctionL() {
	
	// start scheduler and server
	CActiveScheduler *pA=new CActiveScheduler;
	CleanupStack::PushL(pA);
	CActiveScheduler::Install(pA);

	CSoundStreamServer::NewLC();
	
	RSemaphore semaphore;
	User::LeaveIfError(semaphore.OpenGlobal(KSoundStreamServerSemaphoreName));

	// Semaphore opened ok
	semaphore.Signal();
	semaphore.Close();

	// start active scheduler
	CActiveScheduler::Start();

	CleanupStack::PopAndDestroy(2, pA);
}

// gracefully start the server thread
TInt CSoundStreamServer::ThreadFunction(TAny* /*aNone*/) {
    CTrapCleanup* cleanupStack = CTrapCleanup::New();
    
	if (cleanupStack == NULL) {
        PanicServer(ECreateTrapCleanup);
	}

    TRAPD(err, ThreadFunctionL());
    
	if (err != KErrNone) {
        PanicServer(ESrvCreateServer);
    }

    delete cleanupStack;
    cleanupStack = NULL;

    return KErrNone;
}


void CSoundStreamServer::PanicServer(TSoundStreamServerPanic aPanic) {
	RudeDebug *debug = RudeDebug::GetInstance();
	debug->Print("Sound Panic", aPanic);
    User::Panic(KSoundStreamServerName, aPanic);
}

// 
TInt StartThread() {
	TInt result;

	TFindServer findSoundServer(KSoundStreamServerName);
	TFullName name;

	result = findSoundServer.Next(name);
	if (result == KErrNone) {
		// Server already running
		return KErrNone;
	}

	RSemaphore semaphore;
	result = semaphore.CreateGlobal(KSoundStreamServerSemaphoreName, 0);

	if (result != KErrNone) {
		return result;
	}

	RThread thread;

	result=thread.Create(
		KSoundStreamServerName,   // create new server thread
		CSoundStreamServer::ThreadFunction, // thread's main function
		KDefaultStackSize,
		NULL, // share heap with current thread
		&semaphore // passed as TAny* argument to thread function
		);

	if (result != KErrNone) {
		return result;
	}

    thread.SetPriority(EPriorityRealTime); // set thread priority to realtime

	thread.Resume(); // start it going
	thread.Close(); // we're no longer interested in the other thread

	// notify the kernel that a server has started.
	#if defined (__WINS__)
	UserSvr::ServerStarted();
	#endif
	
	// wait for signal from other thread
    semaphore.Wait();
	semaphore.Close();

	return KErrNone;
}

CSoundStreamServerSession::CSoundStreamServerSession(RThread &aClient, CSoundStreamServer* aServer)
: CSession(aClient) {

	// initialize all data structures
	iServer = aServer;
	iStream = CMdaAudioOutputStream::NewL(*this);

	// allocate memory for sound buffer
	iSoundBuf = new TPtr8((TUint8*)iSoundData, SOUNDBUFMAXSIZE*2, SOUNDBUFMAXSIZE*2);

	initialized=false;
	bufferPosition=0;
	
	// remember current time
	timeOfUpdate.UniversalTime( );

	readyForWrite=true;
	iPaused=false;
}


// write to the client thread (used for IPC)
void CSoundStreamServerSession::Write(const TAny* aPtr,const TDesC8& aDes,TInt anOffset) {
	WriteL(aPtr,aDes,anOffset);
}

CSoundStreamServerSession* CSoundStreamServerSession::NewL(RThread &aClient, CSoundStreamServer * aServer) {
	return new(ELeave) CSoundStreamServerSession(aClient,aServer);
}

// process IPC message
void CSoundStreamServerSession::ServiceL(const RMessage& aMessage) {
	TRAPD(err,DispatchMessageL(aMessage));
	aMessage.Complete(err);
}

// shut down everything
CSoundStreamServerSession::~CSoundStreamServerSession() {
	if (initialized) {
		iStream->Stop();
	}

	delete iStream;
	delete iSoundBuf;

	iSoundProviders.Close();
}

// process IPC message
void CSoundStreamServerSession::DispatchMessageL(const RMessage &aMessage) {
	switch (aMessage.Function()) {
		case ESoundStreamServAddSoundProvider: // add sound provider
			AddSoundProvider((ASoundProvider*)aMessage.Ptr0());
			break;
		case ESoundStreamServSetPaused: // pause/unpause
			SetPaused((TBool)aMessage.Int0());
			break;
		case ESoundStreamServOpen: // open
			Open();
			break;
		case ESoundStreamServStop: // stop
			if (initialized) {
				iStream->Stop();
			} else {
				CActiveScheduler::Stop();
			}
			break;
		default:
			// TODO: Panic here (unlikely though)
			break;
	}
}


// ------------------ server/stream functions (CSoundStreamServerSession) -----------------------------


// open stream
void CSoundStreamServerSession::Open() {
	iStream->Open(&iSettings);	
}

// reopen stream
void CSoundStreamServerSession::ReOpen() {
	if (initialized) {
		timeOfUpdate=0;
		iStream->Stop();
		iStream->SetAudioPropertiesL(TMdaAudioDataSettings::ESampleRate16000Hz, 
			TMdaAudioDataSettings::EChannelsMono);

		iStream->SetVolume(iStream->MaxVolume()*3/4);        
		iStream->SetPriority(EPriorityMuchMore, EMdaPriorityPreferenceNone);
	}
}

// callback for iStream->Open()
void CSoundStreamServerSession::MaoscOpenComplete(TInt aError) {
	if (aError==KErrNone) {
        // set stream properties to 16bit,16KHz mono
		iStream->SetAudioPropertiesL(TMdaAudioDataSettings::ESampleRate16000Hz, 
                                     TMdaAudioDataSettings::EChannelsMono);

		iStream->SetVolume(iStream->MaxVolume());        
		iStream->SetPriority(EPriorityMuchMore, EMdaPriorityPreferenceNone);
		
		initialized=true;

		// Start timer (Fill first buffer)
		UpdateBuffer();
	
	} else {
		// should not happen...
		RDebug::Print(_L("Could not init sound!"));
	}
}

// callback
void CSoundStreamServerSession::MaoscBufferCopied(TInt aError, const TDesC8& /*aBuffer*/) {
	readyForWrite=true;

	// underflow? reopen!
	if (aError==KErrUnderflow) {
		ReOpen();
	}
	
	// update next buffer
	if ((aError==KErrUnderflow) || (aError==KErrNone)) {
		UpdateBuffer();
	}
}

// playing stopped
void CSoundStreamServerSession::MaoscPlayComplete(TInt aError) {
	readyForWrite=true;

	timeOfUpdate.UniversalTime();
	
	// if underflow, restart
	if (aError==KErrUnderflow) {
		ReOpen();
		UpdateBuffer();
	} else {
		// no underflow? then it was manually stopped...
		CActiveScheduler::Stop();
	}
}

// update stream buffer
void CSoundStreamServerSession::UpdateBuffer() {
	if (!initialized) return;

	TUint bufferSize;
	TUint i;

	// calculate time since last buffer update
	TTime now;
	now.UniversalTime();

	TTimeIntervalMicroSeconds delta64 = now.MicroSecondsFrom( timeOfUpdate );
	TInt delta = delta64.Int64( ).GetTInt( );

	if ( delta < KSoundInterval )
		delta = KSoundInterval;

	// calculate buffer size
	bufferSize = delta * SOUNDBUFSIZE/KSoundInterval;
	if ( bufferSize > SOUNDBUFMAXSIZE )
		bufferSize = SOUNDBUFMAXSIZE;

	timeOfUpdate.UniversalTime( );
	
	// clear buffer
	for (i=0; i<bufferSize; i++) {
		iSoundData[i]=0;
	}
	
	// if not paused do the calculating
	if (!iPaused) {
		for (i=0; i<(TUint)iSoundProviders.Count(); i++) {
			iSoundProviders[i]->CalcBuffer(&iSoundData[0], bufferSize);
		}
	}

	// Set buffer length, multiply by 2 because we want the size in bytes
	iSoundBuf->SetLength(bufferSize*2);
	
	// write to stream
	readyForWrite=false;
	iStream->WriteL(*iSoundBuf);
}


void CSoundStreamServerSession::SetPaused(TBool paused) {
	iPaused=paused;
}

// ------------------------------- client interface -------------------------------

RSoundStreamClient::RSoundStreamClient() {
}

// Connect to the  server - default number of message slots = 4
TInt RSoundStreamClient::Connect() {
	TInt r=StartThread();
	if (r==KErrNone)
		r=CreateSession(KSoundStreamServerName, TVersion(1,1,1), kDefaultMessageSlots);
	return(r); 
}

// add a sound provider
int RSoundStreamClient::AddSoundProvider(ASoundProvider* a) {
	TAny *p[KMaxMessageArguments];
	p[0]= (TAny*)a;
	return SendReceive(ESoundStreamServAddSoundProvider,&p[0]);
}

// open stream
int RSoundStreamClient::Open() {
	TAny *p[KMaxMessageArguments];
	return SendReceive(ESoundStreamServOpen,&p[0]);
}

// stop stream
int RSoundStreamClient::Stop() {
	TAny *p[KMaxMessageArguments];
	return SendReceive(ESoundStreamServStop,&p[0]);
}

// pause/unpause
int RSoundStreamClient::SetPaused(TBool paused) {
	TInt p[KMaxMessageArguments];
	p[0]= paused;
	return SendReceive(ESoundStreamServSetPaused,&p[0]);
}