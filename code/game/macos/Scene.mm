/*
 Scene.m
 A delegate object used by MyOpenGLView and MainController to render a simple scene.

 Troy Stephens

 Copyright (c) 2003, Apple Computer, Inc., all rights reserved.
*/
/*
 IMPORTANT:  This Apple software is supplied to you by Apple Computer, Inc. ("Apple") in
 consideration of your agreement to the following terms, and your use, installation, 
 modification or redistribution of this Apple software constitutes acceptance of these 
 terms.  If you do not agree with these terms, please do not use, install, modify or 
 redistribute this Apple software.
 
 In consideration of your agreement to abide by the following terms, and subject to these 
 terms, Apple grants you a personal, non-exclusive license, under Apple’s copyrights in 
 this original Apple software (the "Apple Software"), to use, reproduce, modify and 
 redistribute the Apple Software, with or without modifications, in source and/or binary 
 forms; provided that if you redistribute the Apple Software in its entirety and without 
 modifications, you must retain this notice and the following text and disclaimers in all 
 such redistributions of the Apple Software.  Neither the name, trademarks, service marks 
 or logos of Apple Computer, Inc. may be used to endorse or promote products derived from 
 the Apple Software without specific prior written permission from Apple. Except as expressly
 stated in this notice, no other rights or licenses, express or implied, are granted by Apple
 herein, including but not limited to any patent rights that may be infringed by your 
 derivative works or by other works in which the Apple Software may be incorporated.
 
 The Apple Software is provided by Apple on an "AS IS" basis.  APPLE MAKES NO WARRANTIES, 
 EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, 
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS 
 USE AND OPERATION ALONE OR IN COMBINATION WITH YOUR PRODUCTS.
 
 IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR CONSEQUENTIAL 
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
 OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, 
 REPRODUCTION, MODIFICATION AND/OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED AND 
 WHETHER UNDER THEORY OF CONTRACT, TORT (INCLUDING NEGLIGENCE), STRICT LIABILITY OR 
 OTHERWISE, EVEN IF APPLE HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#import "Scene.h"
#import "Texturing.h"

#include "RBGame.h"
#include "RudeDebug.h"
#include "RudeText.h"
#include "RudeFont.h"
#include "RudeGL.h"
#include "RudeTweaker.h"
#include "RudeUnitTest.h"

#include <mach/mach.h>
#include <mach/mach_time.h>
#include <unistd.h>

#import <OpenGL/glu.h>

RBGame *gVBGame = 0;

static double dtor( double degrees )
{
    return degrees * M_PI / 180.0;
}

@implementation Scene

- init
{
    self = [super init];
    if (self) {
        
    }
    return self;
}

- (void)dealloc
{
    [super dealloc];
}


- (void)setViewportRect:(NSRect)bounds
{
	backingWidth = bounds.size.width;
	backingHeight = bounds.size.height;
	
	RGL.SetDeviceWidth(backingWidth);
	RGL.SetDeviceHeight(backingHeight);
}

- (void)flushState
{
	RGL.FlushEnables();
}

- (void)render
{	
	static bool firsttime = true;
	
	if(firsttime)
	{
		
		RudeDebug::Init();
		RudeUnitTest::UnitTest();
		
		RudeFontManager::InitFonts();
		
		RudeText::Init();
		
		if(gVBGame == 0)
			gVBGame = new RBGame();
		
#ifndef NO_RUDETWEAKER
		RudeTweaker::GetInstance()->Init();
#endif
		
		firsttime = false;
	}
	
	static uint64_t lasttime = 0;
	if(lasttime == 0)
		lasttime = mach_absolute_time();
	
	uint64_t thistime = mach_absolute_time();
	uint64_t deltatime = thistime - lasttime;
	lasttime = thistime;
	
	static mach_timebase_info_data_t    sTimebaseInfo = {0,0};
	if ( sTimebaseInfo.denom == 0 ) {
        (void) mach_timebase_info(&sTimebaseInfo);
    }
	
	uint64_t elapsedNano = deltatime * sTimebaseInfo.numer / sTimebaseInfo.denom;
	float elapsedSeconds = ((float) elapsedNano) / 1000000000.0f;
	// Set up rendering state.
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_CULL_FACE );
	
	
	// Clear the framebuffer.
    glClearColor( 0, 0, 0, 0 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	if(gVBGame)
	{
		gVBGame->Render(elapsedSeconds, backingWidth, backingHeight);
	}
	
	glFinish();
	
}

@end
