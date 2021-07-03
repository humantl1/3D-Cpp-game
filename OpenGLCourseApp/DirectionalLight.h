/*
*	Light object with attributes for color, intensity, and direction light is shining
*	This light does not have a location, it is assumed to be distant, e.g. the sun or moon
*/

#pragma once

#include "Light.h"

class DirectionalLight :
	public Light
{
public:
	DirectionalLight();

	DirectionalLight(GLfloat shadowWidth, GLfloat shadowHeight, // shadow map resolution
		GLfloat red, GLfloat green, GLfloat blue,				// Inherit colors from Light class
		GLfloat aIntensity, GLfloat dIntensity,					// Inherit intensities from Light class
		GLfloat xDir, GLfloat yDir, GLfloat zDir);				// Direction light is shining. Unique to DirectionalLight

	// Set uniform variables related to directional light. Called from Shader.SetDirectionalLight
	void UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
		GLuint diffuseIntensityLocation, GLuint directionLocation);

	glm::mat4 CalculateLightTransform();

	~DirectionalLight();

private:
	glm::vec3 direction; // direction light is shining
};

