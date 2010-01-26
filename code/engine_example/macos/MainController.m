/*
 MainController.m
 A controller object that demonstrates full-screen rendering and interaction using the
 NSOpenGL classes.

 Copyright (c) 2003, Apple Computer, Inc., all rights reserved.

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

#import "MainController.h"
#import "MyOpenGLView.h"
#import "Scene.h"

#import <OpenGL/OpenGL.h>

@interface MainController (AnimationMethods)
- (BOOL) isAnimating;
- (void) startAnimation;
- (void) stopAnimation;
- (void) toggleAnimation;

- (void) startAnimationTimer;
- (void) stopAnimationTimer;
- (void) animationTimerFired:(NSTimer *)timer;
@end

@implementation MainController

- (void) awakeFromNib
{
    isAnimating = NO;
    [self startAnimation];
}

// Action method wired up to fire when the user clicks the "Go FullScreen" button.  We remain in this method until the user exits FullScreen mode.
- (IBAction) goFullScreen:(id)sender
{
    Scene *scene = [openGLView scene];
    CFAbsoluteTime timeNow;
    CGLContextObj cglContext;
    CGDisplayErr err;
    long oldSwapInterval;
    long newSwapInterval;

    // Pixel Format Attributes for the FullScreen NSOpenGLContext
    NSOpenGLPixelFormatAttribute attrs[] = {

        // Specify that we want a full-screen OpenGL context.
        NSOpenGLPFAFullScreen,

        // We may be on a multi-display system (and each screen may be driven by a different renderer), so we need to specify which screen we want to take over.  For this demo, we'll specify the main screen.
        NSOpenGLPFAScreenMask, CGDisplayIDToOpenGLDisplayMask(kCGDirectMainDisplay),

        // Attributes Common to FullScreen and non-FullScreen
        NSOpenGLPFAColorSize, 24,
        NSOpenGLPFADepthSize, 16,
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFAAccelerated,
        0
    };
    long rendererID;

    // Create the FullScreen NSOpenGLContext with the attributes listed above.
    NSOpenGLPixelFormat *pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
    
    // Just as a diagnostic, report the renderer ID that this pixel format binds to.  CGLRenderers.h contains a list of known renderers and their corresponding RendererID codes.
    [pixelFormat getValues:&rendererID forAttribute:NSOpenGLPFARendererID forVirtualScreen:0];
    NSLog(@"FullScreen pixelFormat RendererID = %08x", (unsigned)rendererID);

    // Create an NSOpenGLContext with the FullScreen pixel format.  By specifying the non-FullScreen context as our "shareContext", we automatically inherit all of the textures, display lists, and other OpenGL objects it has defined.
    fullScreenContext = [[NSOpenGLContext alloc] initWithFormat:pixelFormat shareContext:[openGLView openGLContext]];
    [pixelFormat release];
    pixelFormat = nil;

    if (fullScreenContext == nil) {
        NSLog(@"Failed to create fullScreenContext");
        return;
    }

    // Pause animation in the OpenGL view.  While we're in full-screen mode, we'll drive the animation actively instead of using a timer callback.
    if ([self isAnimating]) {
        [self stopAnimationTimer];
    }

    // Take control of the display where we're about to go FullScreen.
    err = CGCaptureAllDisplays();
    if (err != CGDisplayNoErr) {
        [fullScreenContext release];
        fullScreenContext = nil;
        return;
    }

    // Enter FullScreen mode and make our FullScreen context the active context for OpenGL commands.
    [fullScreenContext setFullScreen];
    [fullScreenContext makeCurrentContext];

    // Save the current swap interval so we can restore it later, and then set the new swap interval to lock us to the display's refresh rate.
    cglContext = CGLGetCurrentContext();
    CGLGetParameter(cglContext, kCGLCPSwapInterval, &oldSwapInterval);
    newSwapInterval = 1;
    CGLSetParameter(cglContext, kCGLCPSwapInterval, &newSwapInterval);

    // Tell the scene the dimensions of the area it's going to render to, so it can set up an appropriate viewport and viewing transformation.
    [scene setViewportRect:NSMakeRect(0, 0, CGDisplayPixelsWide(kCGDirectMainDisplay), CGDisplayPixelsHigh(kCGDirectMainDisplay))];
	
	// Tell the scene to flush cached OpenGL state (which isn't maintained when we go fullscreen)
	[scene flushState];

    // Now that we've got the screen, we enter a loop in which we alternately process input events and computer and render the next frame of our animation.  The shift here is from a model in which we passively receive events handed to us by the AppKit to one in which we are actively driving event processing.
    timeBefore = CFAbsoluteTimeGetCurrent();
    stayInFullScreenMode = YES;
    while (stayInFullScreenMode) {
        NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

        // Check for and process input events.
        NSEvent *event;
        while (event = [NSApp nextEventMatchingMask:NSAnyEventMask untilDate:[NSDate distantPast] inMode:NSDefaultRunLoopMode dequeue:YES]) {
            switch ([event type]) {
                case NSLeftMouseDown:
                    [self mouseDown:event];
                    break;

                case NSLeftMouseUp:
                    [self mouseUp:event];
                    break;

                case NSLeftMouseDragged:
                    [self mouseDragged:event];
                    break;

                case NSKeyDown:
                    [self keyDown:event];
                    break;

                default:
                    break;
            }
        }

        // Update our animation.
        timeNow = CFAbsoluteTimeGetCurrent();
        timeBefore = timeNow;

        // Render a frame, and swap the front and back buffers.
        [scene render];
        [fullScreenContext flushBuffer];

        // Clean up any autoreleased objects that were created this time through the loop.
        [pool release];
    }
    
    // Clear the front and back framebuffers before switching out of FullScreen mode.  (This is not strictly necessary, but avoids an untidy flash of garbage.)
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    [fullScreenContext flushBuffer];
    glClear(GL_COLOR_BUFFER_BIT);
    [fullScreenContext flushBuffer];

    // Restore the previously set swap interval.
    CGLSetParameter(cglContext, kCGLCPSwapInterval, &oldSwapInterval);

    // Exit fullscreen mode and release our FullScreen NSOpenGLContext.
    [NSOpenGLContext clearCurrentContext];
    [fullScreenContext clearDrawable];
    [fullScreenContext release];
    fullScreenContext = nil;

    // Release control of the display.
    CGReleaseAllDisplays();

    // Mark our view as needing drawing.  (The animation has advanced while we were in FullScreen mode, so its current contents are stale.)
    [openGLView setNeedsDisplay:YES];

    // Resume animation timer firings.
    if ([self isAnimating]) {
        [self startAnimationTimer];
    }
}

- (void) keyDown:(NSEvent *)event
{
    unichar c = [[event charactersIgnoringModifiers] characterAtIndex:0];

	switch (c) {
			
			// [Esc] exits FullScreen mode.
        case 27:
            stayInFullScreenMode = NO;
            break;
			
        default:
            break;
    }
}

// Holding the mouse button and dragging the mouse changes the "roll" angle (y-axis) and the direction from which sunlight is coming (x-axis).
- (void)mouseDown:(NSEvent *)theEvent
{
    //NSPoint lastWindowPoint = [theEvent locationInWindow];
}

- (BOOL) isInFullScreenMode
{
    return fullScreenContext != nil;
}

@end

@implementation MainController (AnimationMethods)

- (BOOL) isAnimating
{
    return isAnimating;
}

- (void) startAnimation
{
    if (!isAnimating) {
        isAnimating = YES;
        if (![self isInFullScreenMode]) {
            [self startAnimationTimer];
        }
    }
}

- (void) stopAnimation
{
    if (isAnimating) {
        if (animationTimer != nil) {
            [self stopAnimationTimer];
        }
        isAnimating = NO;
    }
}

- (void) toggleAnimation
{
    if ([self isAnimating]) {
        [self stopAnimation];
    } else {
        [self startAnimation];
    }
}

- (void) startAnimationTimer
{
    if (animationTimer == nil) {
        animationTimer = [[NSTimer scheduledTimerWithTimeInterval:0.017 target:self selector:@selector(animationTimerFired:) userInfo:nil repeats:YES] retain];
    }
}

- (void) stopAnimationTimer
{
    if (animationTimer != nil) {
        [animationTimer invalidate];
        [animationTimer release];
        animationTimer = nil;
    }
}

- (void) animationTimerFired:(NSTimer *)timer
{
    [openGLView setNeedsDisplay:YES];
}

@end

