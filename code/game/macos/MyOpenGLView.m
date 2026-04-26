/*
     File: MyOpenGLView.m
 Abstract:  An NSOpenGLView subclass that delegates to separate "scene" 
 and "controller" objects for OpenGL rendering and input event handling.
*/

#import "MyOpenGLView.h"
#import "MainController.h"
#import "Scene.h"

@implementation MyOpenGLView

- (void) setMainController:(MainController*)theController;
{
	controller = theController;
}

- (NSOpenGLContext*) openGLContext
{
    return [super openGLContext];
}

- (CVReturn) getFrameForTime:(const CVTimeStamp*)outputTime
{
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
	CFAbsoluteTime currentTime = CFAbsoluteTimeGetCurrent();
	[[controller scene] advanceTimeBy:(currentTime - [controller renderTime])];
	[controller setRenderTime:currentTime];
	
	[self drawView];
	
	[pool release];
    return kCVReturnSuccess;
}

static CVReturn MyDisplayLinkCallback(CVDisplayLinkRef displayLink, const CVTimeStamp* now, const CVTimeStamp* outputTime, CVOptionFlags flagsIn, CVOptionFlags* flagsOut, void* displayLinkContext)
{
    CVReturn result = [(MyOpenGLView*)displayLinkContext getFrameForTime:outputTime];
    return result;
}

- (void) setupDisplayLink
{
	CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);
	CVDisplayLinkSetOutputCallback(displayLink, &MyDisplayLinkCallback, self);
	
	CGLContextObj cglContext = [[self openGLContext] CGLContextObj];
	CGLPixelFormatObj cglPixelFormat = [[self pixelFormat] CGLPixelFormatObj];
	CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(displayLink, cglContext, cglPixelFormat);
}

- (void) prepareOpenGL
{
    [super prepareOpenGL];
    
    [[self openGLContext] makeCurrentContext];
    
    GLint swapInt = 1;
    [[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];
    
    if(!displayLink)
        [self setupDisplayLink];
}

- (id) initWithFrame:(NSRect)frameRect shareContext:(NSOpenGLContext*)context
{
    NSOpenGLPixelFormatAttribute attribs[] =
    {
		kCGLPFAAccelerated,
		kCGLPFANoRecovery,
		kCGLPFADoubleBuffer,
		kCGLPFAColorSize, 24,
		kCGLPFADepthSize, 16,
		0
    };
	
    NSOpenGLPixelFormat *pixelFormat = [[[NSOpenGLPixelFormat alloc] initWithAttributes:attribs] autorelease];
	
    if (!pixelFormat)
		NSLog(@"No OpenGL pixel format");
	
	if (self = [super initWithFrame:frameRect pixelFormat:pixelFormat]) {
        if (context) {
            [self setOpenGLContext:[[[NSOpenGLContext alloc] initWithFormat:pixelFormat shareContext:context] autorelease]];
        }
        
        [self setWantsLayer:YES];
        [self setWantsBestResolutionOpenGLSurface:YES];
	}
	
	return self;
}

- (id) initWithFrame:(NSRect)frameRect
{
	self = [self initWithFrame:frameRect shareContext:nil];
	return self;
}

- (void) reshape
{
    [super reshape];
    
	CGLLockContext([[self openGLContext] CGLContextObj]);
	
    NSRect backingBounds = [self convertRectToBacking:[self bounds]];
	[[controller scene] setViewportRect:backingBounds];
	[[self openGLContext] update];
	
	CGLUnlockContext([[self openGLContext] CGLContextObj]);
    
    [self drawView];
}

- (void) drawRect:(NSRect)dirtyRect
{
	[self drawView];
}

- (void) drawView
{
	CGLLockContext([[self openGLContext] CGLContextObj]);
	
	[[self openGLContext] makeCurrentContext];
	
    [[controller scene] render];
	[[self openGLContext] flushBuffer];
	
	CGLUnlockContext([[self openGLContext] CGLContextObj]);
}
 
- (BOOL) acceptsFirstResponder
{
    return YES;
}

- (void) keyDown:(NSEvent *)theEvent
{
    [controller keyDown:theEvent];
}

- (void)mouseDown:(NSEvent *)theEvent
{
    [controller mouseDown:theEvent];
}

- (void)mouseUp:(NSEvent *)theEvent
{
    [controller mouseUp:theEvent];
}

- (void)mouseDragged:(NSEvent *)theEvent
{
    [controller mouseDragged:theEvent];
}

- (void)scrollWheel:(NSEvent *)theEvent
{
    [controller scrollWheel:theEvent];
}

- (void) startAnimation
{
	if (!displayLink)
		[self setupDisplayLink];
	
	if (displayLink && !CVDisplayLinkIsRunning(displayLink))
		CVDisplayLinkStart(displayLink);
}

- (void) stopAnimation
{
	if (displayLink && CVDisplayLinkIsRunning(displayLink))
		CVDisplayLinkStop(displayLink);
}

- (void) dealloc
{
	CVDisplayLinkStop(displayLink);
    CVDisplayLinkRelease(displayLink);
	
	[super dealloc];
}	

@end
