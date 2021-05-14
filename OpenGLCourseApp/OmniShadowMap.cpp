#include "OmniShadowMap.h"

OmniShadowMap::OmniShadowMap() : ShadowMap() {}

// 1. Generate Framebuffer FBO and shadow map texture 
// 2. Set shadow map as active and bind to a cube map
// *3. Define each layer (side) of the shadowMap cube map
// 4. Set shadowMap border and mipmap level-of-detail parameters
// 5. Bind FBO as active frame buffer
// *6. Set FBO to output Depth data to shadowMap when updated
// 7. Specify that color data is not being read or written
// 8. Check for error status
// 9. Unbind FBO 
bool OmniShadowMap::Init(GLuint width, GLuint height)
{
	// shadow map texture height and width. Should match viewport h&w
	shadowWidth = width;
	shadowHeight = height;

// 1.
	// create a framebuffer (screen buffer)
	// draw to this buffer then save as texture to be used on the default frame buffer
	glGenFramebuffers(1, &FBO);						// create 1 frame buffer, pass ID to FBO
	glGenTextures(1, &shadowMap);					// create 1 texture, pass ID to shadowMap

// 2.
	glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMap);  // bind shadow map to a cube map

// *3.
	// Define each cube map layer texture image, allowing elements of an image array to be read by shaders
	for (size_t i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,	// GL_TEXTURE_CUBE_MAP_... are consecutive enums referencing cube map sides.
			0,												// mipmap level-of-detail. 0 is base image level.  
			GL_DEPTH_COMPONENT,								// number of components in texture. depth component is just 1
			shadowWidth,  									// width of texture image 
			shadowHeight, 									// height of texture image
			0,            									// border value (must be 0 because border is defined in glTexParameteri) 
			GL_DEPTH_COMPONENT,								// format of of pixel data
			GL_FLOAT,     									// data type of pixel data
			nullptr);      									// pointer to the image data in memory. The image that will be passed to this will be created in the FBO
	}

// 4.
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);			// set mipmap minifying function to linear
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);			// set mipmap magnifying function to linear
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);		// clamp value to edge, not border, since light is omnidirectional and covers every direction 
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);		// clamp value to edge, not border, since light is omnidirectional and covers every direction 
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);		// R represents Z axis, helps with corners and edges

// 5.
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);										// set FBO as active frame buffer 

// 6.
	// This is not Texture2D like ShadowMap, because we are writing to a cube map
	glFramebufferTexture(GL_FRAMEBUFFER,      // What we are writing from
						GL_DEPTH_ATTACHMENT,  // Which part of frame buffer should be attached (could also be color, alpha, etc)
						shadowMap,            // ID of texture to write to
						0);                   // mipmap level               

// 7.
	// it is necessary to explicitly specify we are NOT writing color data
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

// 8.
	// error checking
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Framebuffer Error: %i", status);
		return false;
	}

// 9.
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // 0 is the default frame buffer for the screen
	return true;

}                                          

void OmniShadowMap::Write()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO); // Write to the offscreen framebuffer FBO
}

void OmniShadowMap::Read(GLenum textureUnit)
{
	glActiveTexture(textureUnit);					// set the shadow map to read from GL_TEXTURE_CUBE_MAP_...
	glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMap);
}

OmniShadowMap::~OmniShadowMap() {}
