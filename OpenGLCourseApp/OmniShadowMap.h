#pragma once
#include "ShadowMap.h"
class OmniShadowMap :
    public ShadowMap
{
public:
	OmniShadowMap();

	bool Init(GLuint width, GLuint height);

	// Bind FBO (frame buffer object) to write to the shadow map
	void Write();

	// Use when writing to the screen and using the shadow map as a texture reference
	// textureUnit specifies WHICH texture unit is being used
	void Read(GLenum textureUnit);

	~OmniShadowMap();
};

