#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity);  // could also pass in vec with values. These are bound to a shader uniform 

	~Light();

protected:
	glm::vec3 color;
	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;
};

