#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp >

#include "ShadowMap.h"

class Light
{
public:
	Light();
	Light(GLuint shadowWidth, GLuint shadowHeight, // width and height of shadow map
		GLfloat red, GLfloat green, GLfloat blue,    // could also pass in vec3
		GLfloat aIntensity, GLfloat dIntensity);	 // Ambient intensity illuminates everything equally, diffuse intensity illuminates based on angle of face

	ShadowMap* GetShadowMap() { return shadowMap; }

	~Light();

protected:
	glm::vec3 color;
	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;

	glm::mat4 lightProj; // what the light "sees"

	ShadowMap* shadowMap;
};

