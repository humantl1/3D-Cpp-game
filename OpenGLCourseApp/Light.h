#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, // could also pass in vec with values.
		GLfloat aIntensity, GLfloat dIntensity);	// These are bound to a shader uniform 

	~Light();

protected:
	glm::vec3 color;
	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;
};

