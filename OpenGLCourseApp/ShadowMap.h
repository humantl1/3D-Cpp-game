#pragma once

#include <stdio.h>

#include <GL/glew.h>

class ShadowMap
{
public:
	ShadowMap();

	virtual bool Init(GLuint width, GLuint height);

	// Bind FBO (frame buffer object) to write to the shadow map
	virtual void Write();

	// Use when writing to the screen and using the shadow map as a texture reference
	// textureUnit specifies WHICH texture unit is being used
	virtual void Read(GLenum textureUnit);

	inline GLuint GetShadowWidth() { return shadowWidth; }
	inline GLuint GetShadowHeight() { return shadowHeight; }

	~ShadowMap();


protected:
	GLuint FBO, shadowMap;				// frame buffer ID, shadow map texture ID
	GLuint shadowWidth, shadowHeight;	// viewport should be the same size as the shadow map texture
};

