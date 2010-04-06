/*

File: EAGLView.m
Abstract: This class wraps the CAEAGLLayer from CoreAnimation into a convenient
UIView subclass. The view content is basically an EAGL surface you render your
OpenGL scene into.  Note that setting the view non-opaque will only work if the
EAGL surface has an alpha channel.

Version: 1.7

Disclaimer: IMPORTANT:  This Apple software is supplied to you by Apple Inc.
("Apple") in consideration of your agreement to the following terms, and your
use, installation, modification or redistribution of this Apple software
constitutes acceptance of these terms.  If you do not agree with these terms,
please do not use, install, modify or redistribute this Apple software.

In consideration of your agreement to abide by the following terms, and subject
to these terms, Apple grants you a personal, non-exclusive license, under
Apple's copyrights in this original Apple software (the "Apple Software"), to
use, reproduce, modify and redistribute the Apple Software, with or without
modifications, in source and/or binary forms; provided that if you redistribute
the Apple Software in its entirety and without modifications, you must retain
this notice and the following text and disclaimers in all such redistributions
of the Apple Software.
Neither the name, trademarks, service marks or logos of Apple Inc. may be used
to endorse or promote products derived from the Apple Software without specific
prior written permission from Apple.  Except as expressly stated in this notice,
no other rights or licenses, express or implied, are granted by Apple herein,
including but not limited to any patent rights that may be infringed by your
derivative works or by other works in which the Apple Software may be
incorporated.

The Apple Software is provided by Apple on an "AS IS" basis.  APPLE MAKES NO
WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED
WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE OR IN
COMBINATION WITH YOUR PRODUCTS.

IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION, MODIFICATION AND/OR
DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER UNDER THEORY OF
CONTRACT, TORT (INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN IF
APPLE HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Copyright (C) 2008 Apple Inc. All Rights Reserved.

*/

#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGLDrawable.h>
#include <mach/mach.h>
#include <mach/mach_time.h>
#include <unistd.h>

#import "EAGLView.h"

#include "RBGame.h"
#include "RudeGL.h"
#include "RudeDebug.h"
#include "RudeText.h"
#include "RudeFont.h"
#include "RudeTweaker.h"
#include "RudeUnitTest.h"

RBGame *gVBGame = 0;
bool gLandscape = false;

@interface EAGLView (EAGLViewPrivate)

- (BOOL)createFramebuffer;
- (void)destroyFramebuffer;

@end

@interface EAGLView (EAGLViewSprite)

- (void)setupView;

@end

@implementation EAGLView

@synthesize animationInterval;

// You must implement this
+ (Class) layerClass
{
	return [CAEAGLLayer class];
}


//The GL view is stored in the nib file. When it's unarchived it's sent -initWithCoder:
- (id)initWithCoder:(NSCoder*)coder
{
	RudeDebug::Init();
	RudeUnitTest::UnitTest();
	
	if((self = [super initWithCoder:coder])) {
		// Get the layer
		CAEAGLLayer *eaglLayer = (CAEAGLLayer*) self.layer;
		
		eaglLayer.opaque = YES;
		eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
										[NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
		
		context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
		
		RUDE_REPORT("initWithCoder\n");
		
		if(!context || ![EAGLContext setCurrentContext:context] || ![self createFramebuffer]) {
			RUDE_REPORT("FAIL..\n");
			[self release];
			return nil;
		}
		
		animationInterval = 1.0 / 30.0;
		
		[self setupView];
		[self drawView];
	}
	
	RudeFontManager::InitFonts();
	
	if(gVBGame == 0)
		gVBGame = new RBGame();
	
	
	RudeText::Init();
	
#ifndef NO_RUDETWEAKER
	RudeTweaker::GetInstance()->Init();
#endif
	
	return self;
}


- (void)layoutSubviews
{
	RUDE_REPORT("layoutSubviews 0\n");
	
	[EAGLContext setCurrentContext:context];
	[self destroyFramebuffer];
	[self createFramebuffer];
	[self drawView];
	
	RUDE_REPORT("layoutSubviews 1\n");
}


- (BOOL)createFramebuffer
{
	RUDE_REPORT("createFramebuffer 0\n");
	
	if(![EAGLContext setCurrentContext:context]) {
		return NO;
	}
	
	glGenFramebuffersOES(1, &viewFramebuffer);
	glGenRenderbuffersOES(1, &viewRenderbuffer);
	glGenRenderbuffersOES(1, &depthRenderbuffer);
	
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
	[context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:(id<EAGLDrawable>)self.layer];
	glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, viewRenderbuffer);
	
	glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &backingWidth);
	glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &backingHeight);
	
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, depthRenderbuffer);
	glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT24_OES, backingWidth, backingHeight);
	glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, depthRenderbuffer);
	
	if(glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES) {
		NSLog(@"failed to make complete framebuffer object %x", glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES));
		return NO;
	}
	
	RUDE_REPORT("createFramebuffer 1\n");
	
	return YES;
}


- (void)destroyFramebuffer
{
	RUDE_REPORT("destroyFramebuffer\n");
	
	EAGLContext *oldContext = [EAGLContext currentContext];
	
	if (oldContext != context)
		[EAGLContext setCurrentContext:context];
	
	if(depthRenderbuffer) {
		glDeleteRenderbuffersOES(1, &depthRenderbuffer);
		depthRenderbuffer = 0;
	}
	
	glDeleteRenderbuffersOES(1, &viewRenderbuffer);
	viewRenderbuffer = 0;
	
	glDeleteFramebuffersOES(1, &viewFramebuffer);
	viewFramebuffer = 0;
	
	if (oldContext != context)
		[EAGLContext setCurrentContext:oldContext];
}


- (void)startAnimation
{
	animationTimer = [NSTimer scheduledTimerWithTimeInterval:animationInterval target:self selector:@selector(drawView) userInfo:nil repeats:YES];
}


- (void)stopAnimation
{
	[animationTimer invalidate];
	animationTimer = nil;
}


- (void)setAnimationInterval:(NSTimeInterval)interval
{
	animationInterval = interval;
	
	if(animationTimer) {
		[self stopAnimation];
		[self startAnimation];
	}
}


// Sets up an array of values to use as the sprite vertices.
const GLfloat spriteVertices[] = {
-0.5f, -0.5f,
0.5f, -0.5f,
-0.5f,  0.5f,
0.5f,  0.5f,
};

// Sets up an array of values for the texture coordinates.
const GLshort spriteTexcoords[] = {
0, 0,
1, 0,
0, 1,
1, 1,
};

- (void)setupView
{
	RGL.SetDeviceWidth(backingWidth);
	RGL.SetDeviceHeight(backingHeight);
	
	
	// Sets up matrices and transforms for OpenGL ES
	// 320x480
	glViewport(0, 0, backingHeight, backingWidth);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrthof(-1.0f, 1.0f, -1.5f, 1.5f, -1.0f, 1.0f);
	glOrthof(0, backingHeight, 0, backingWidth, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	
	//glScalef(1.0f, -1.0f, 1.0f);
	//glTranslatef(0.0f, -backingHeight, 0.0f);
	
	// Clears the view with black
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	//glEnable(GL_LINE_SMOOTH);
	//glLineWidth(1.0f);
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	
	glDepthFunc(GL_LEQUAL);
	
	/*
	// Sets up pointers and enables states needed for using vertex arrays and textures
	glVertexPointer(2, GL_FLOAT, 0, spriteVertices);
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_SHORT, 0, spriteTexcoords);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	// Creates a Core Graphics image from an image file
	spriteImage = [UIImage imageNamed:@"Sprite.png"].CGImage;
	// Get the width and height of the image
	width = CGImageGetWidth(spriteImage);
	height = CGImageGetHeight(spriteImage);
	// Texture dimensions must be a power of 2. If you write an application that allows users to supply an image,
	// you'll want to add code that checks the dimensions and takes appropriate action if they are not a power of 2.

	if(spriteImage) {
		// Allocated memory needed for the bitmap context
		spriteData = (GLubyte *) malloc(width * height * 4);
		// Uses the bitmatp creation function provided by the Core Graphics framework. 
		spriteContext = CGBitmapContextCreate(spriteData, width, height, 8, width * 4, CGImageGetColorSpace(spriteImage), kCGImageAlphaPremultipliedLast);
		// After you create the context, you can draw the sprite image to the context.
		CGContextDrawImage(spriteContext, CGRectMake(0.0, 0.0, (CGFloat)width, (CGFloat)height), spriteImage);
		// You don't need the context at this point, so you need to release it to avoid memory leaks.
		CGContextRelease(spriteContext);
		
		// Use OpenGL ES to generate a name for the texture.
		glGenTextures(1, &spriteTexture);
		// Bind the texture name. 
		glBindTexture(GL_TEXTURE_2D, spriteTexture);
		// Speidfy a 2D texture image, provideing the a pointer to the image data in memory
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, spriteData);
		// Release the image data
		free(spriteData);
		
		// Set the texture parameters to use a minifying filter and a linear filer (weighted average)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		
		// Enable use of the texture
		glEnable(GL_TEXTURE_2D);
		// Set a blending function to use
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		// Enable blending
		glEnable(GL_BLEND);
	}
	 */
}

// Updates the OpenGL view when the timer fires
- (void)drawView
{
	// Make sure that you are drawing to the current context
	[EAGLContext setCurrentContext:context];
	
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
	//glRotatef(3.0f, 0.0f, 0.0f, 1.0f);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
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
	
	
	if(gVBGame)
	{
		float aspect = ((float) backingWidth) / ((float) backingHeight);

		gVBGame->Render(elapsedSeconds, backingWidth, backingHeight);
		
	}
	
	
	
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
	[context presentRenderbuffer:GL_RENDERBUFFER_OES];
}

// Stop animating and release resources when they are no longer needed.
- (void)dealloc
{
	[self stopAnimation];
	
	[self destroyFramebuffer];
	
	if([EAGLContext currentContext] == context) {
		[EAGLContext setCurrentContext:nil];
	}
	
	[context release];
	context = nil;
	
	[super dealloc];
}


- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
	
	for (UITouch *touch in touches) {
		
		CGPoint touchPoint = [touch locationInView:self];
		
		RudeScreenVertex touchDown;
		touchDown.m_x = touchPoint.x;
		touchDown.m_y = touchPoint.y;
		
		gVBGame->TouchDown(touchDown);
	}
	
}


- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
	
	for (UITouch *touch in touches) {
		
		CGPoint touchPoint = [touch locationInView:self];
		CGPoint prevPoint = [touch previousLocationInView:self];
		
		RudeScreenVertex touchDown;
		touchDown.m_x = touchPoint.x;
		touchDown.m_y = touchPoint.y;
		
		RudeScreenVertex prevDown;
		prevDown.m_x = prevPoint.x;
		prevDown.m_y = prevPoint.y;

		gVBGame->TouchMove(touchDown, prevDown);
	}
}


- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
	
	for (UITouch *touch in touches) {
		
		CGPoint touchPoint = [touch locationInView:self];
		CGPoint prevPoint = [touch previousLocationInView:self];
		
		RudeScreenVertex touchDown;
		touchDown.m_x = touchPoint.x;
		touchDown.m_y = touchPoint.y;
		
		RudeScreenVertex prevDown;
		prevDown.m_x = prevPoint.x;
		prevDown.m_y = prevPoint.y;
		
		gVBGame->TouchUp(touchDown, prevDown);
	}
}



@end
