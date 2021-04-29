#include "ShadowMap.h"

ShadowMap::ShadowMap() : FBO{0}, shadowMap{0} {}

// 1. Generate Framebuffer FBO and Texture shadowMap
// 2. Bind shadowMap as active texture
// *3. Define shadowMap
// 4. Set shadowMap border and mipmap level-of-detail parameters
// 5. Bind FBO as active frame buffer (might be able to do this @ step 2
// *6. Set FBO to output Depth data to shadowMap when updated
// 7. Specify that color data is not being read or written
// 8. Check for error status
// 9. Unbind FBO 
bool ShadowMap::Init(GLuint width, GLuint height)
{
	shadowWidth = width;
	shadowHeight = height;

	// create a framebuffer (screen buffer)
	// draw to this buffer then save as texture to be used on the default frame buffer
	glGenFramebuffers(1, &FBO); // create 1, pass ID to FBO

	glGenTextures(1, &shadowMap);			 // create 1, pass ID to shadowMap
	glBindTexture(GL_TEXTURE_2D, shadowMap); // set shadowMap as active texture

	// Define texture image, allowing elements of an image array to be read by shaders
	glTexImage2D(GL_TEXTURE_2D,				 // target texture (shadowMap via GL_TEXTURE_2D) 
					0,						 // mipmap level-of-detail. 0 is base image level.  
					GL_DEPTH_COMPONENT,		 // number of components in texture. depth component is just 1 
					shadowWidth,			 // width of texture image 
					shadowHeight,			 // height of texture image
					0,						 // border value (must be 0 because border is defined in glTexParameteri) 
					GL_DEPTH_COMPONENT,		 // format of of pixel data 
					GL_FLOAT,				 // data type of pixel data
					nullptr);				 // pointer to the image data in memory. The image that will be passed to this will be created in the FBO
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);		// clamp value beyond border of shadow map
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);		// clamp value beyond border of shadow map
	float bColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };									// set the border "color"
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, bColor);			// this sets the value to 1 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);			// set mipmap minifying function to linear
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);			// set mipmap magnifying function to linear

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);  // set FBO as active frame buffer TODO: I think this can be bound when the texture is bound. Check
	
	// "bind" shadow map (via GL_TEXTURE_2D) to FBO, so that everytime the FBO is updated, the image will be output to the shadow map texture
	glFramebufferTexture2D(GL_FRAMEBUFFER,	 // What we are writing from
						GL_DEPTH_ATTACHMENT, // Which part of frame buffer should be attached (could also be color, alpha, etc)
						GL_TEXTURE_2D,		 // Format of data to write to
						shadowMap,			 // ID of texture to write to
						0);					 // mipmap level

	// it is necessary to explicitly specify we are NOT writing color data
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	// error checking
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Framebuffer Error: %i", status);
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0); // 0 is the default frame buffer for the screen
	return true;
}

void ShadowMap::Write()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO); // Write to the offscreen framebuffer FBO
}

void ShadowMap::Read(GLenum textureUnit)
{
	glActiveTexture(textureUnit);			 // set the shadow map to read from
	glBindTexture(GL_TEXTURE_2D, shadowMap);
}

// delete framebuffer and shadow map
ShadowMap::~ShadowMap()
{
	if (FBO)
	{
		glDeleteFramebuffers(1, &FBO);
	}

	if (shadowMap)
	{
		glDeleteTextures(1, &shadowMap);
	}
}
