

#include "RudeTexture.h"
#include "RudeFile.h"
#include "RudeDebug.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <CoreFoundation/CFBundle.h>
#include <CoreGraphics/CGContext.h>
#include <CoreGraphics/CGBitmapContext.h>
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>

#include "PVRTTexture.h"
#include "PVRTTextureAPI.h"

RudeTexture::RudeTexture()
: m_height(0)
, m_width(0)
, m_texture(0)
{
	m_name[0] = '\0';
}

RudeTexture::~RudeTexture()
{
}

int RudeTexture::LoadFromPVRTFile(const char *name)
{
	RUDE_REPORT("LoadFromPVRTFile %s\n", name);
	
	strncpy(m_name, name, kNameLen);
	
	char filename[kNameLen + 4];
	sprintf(filename, "%s.pvr", name);
	
	char filepath[512];
	RudeFileGetFile(filename, filepath, 512);
	
	PVR_Texture_Header header;

	unsigned int result = PVRTLoadTextureFromPVR(filepath, &m_texture, &header);
	
	if(result == 0)
		return -1;
	
	m_height = header.dwHeight;
	m_width = header.dwWidth;
	
	return 0;

}

int RudeTexture::LoadFromPVRTPointer(const char *name, const void *data)
{
	strncpy(m_name, name, kNameLen);
	
	PVR_Texture_Header header;
	
	int result = PVRTLoadTextureFromPointer(data, &m_texture, &header);
	
	if(result == 0)
		return -1;
	
	m_height = header.dwHeight;
	m_width = header.dwWidth;
	
	return 0;
}

int RudeTexture::LoadFromPNG(const char *name)
{	
	int result = -1;
	
	strncpy(m_name, name, kNameLen);
	
	char filename[64];
	sprintf(filename, "%s.png", name);
	
	CFStringRef cfFilename = CFStringCreateWithCString(0, filename, kCFStringEncodingASCII);
	CFBundleRef mainBundle = CFBundleGetMainBundle();
	CFURLRef url = CFBundleCopyResourceURL(mainBundle, cfFilename, 0, 0);
	
	if(url == NULL)
		goto LoadFromPNG_URLFail;
	
	CGDataProviderRef ref = CGDataProviderCreateWithURL(url);
	if(ref == NULL)
		goto LoadFromPNG_URLProviderFail;
	
	CGImageRef image = CGImageCreateWithPNGDataProvider(ref, 0, false, kCGRenderingIntentDefault);
	if(image == NULL)
		goto LoadFromPNG_ImageRefFail;
	
	CGDataProviderRelease(ref);
	
	m_width = CGImageGetWidth(image);
	m_height = CGImageGetHeight(image);
	
	GLubyte *imageData = (GLubyte *) malloc(m_width * m_height * 4);
	
	CGContextRef imageContext = CGBitmapContextCreate(imageData, m_width, m_height, 8, m_width * 4, CGImageGetColorSpace(image), kCGImageAlphaPremultipliedLast);
	CGContextDrawImage(imageContext, CGRectMake(0.0, 0.0, (CGFloat) m_width, (CGFloat) m_height), image);
	CGContextRelease(imageContext);
	
	
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
	free(imageData);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	result = 0;
	
	CFRelease(image);
	
LoadFromPNG_ImageRefFail:
	
	
LoadFromPNG_URLProviderFail:
	CFRelease(url);
	
LoadFromPNG_URLFail:
	CFRelease(cfFilename);
	
	return result;
}

void RudeTexture::SetActive()
{
	
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glEnable(GL_TEXTURE_2D);
	//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_BLEND);
	
	
}

