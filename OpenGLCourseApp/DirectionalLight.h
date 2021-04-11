#pragma once
#include "Light.h"

class DirectionalLight :
	public Light
{
public:
	DirectionalLight();

	DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, // could also pass in vec with values. 
		GLfloat aIntensity, GLfloat dIntensity, 
		GLfloat xDir, GLfloat yDir, GLfloat zDir); // these handle diffuse lighting

	void UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
		GLuint diffuseIntensityLocation, GLuint directionLocation);

	~DirectionalLight();

private:
	glm::vec3 direction;
};

