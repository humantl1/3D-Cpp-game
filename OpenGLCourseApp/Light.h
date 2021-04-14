#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, // could also pass in vec3
		GLfloat aIntensity, GLfloat dIntensity);	// Ambient intensity illuminates everything equally, diffuse intensity illuminates based on angle of face

	~Light();

protected:
	glm::vec3 color;
	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;
};

