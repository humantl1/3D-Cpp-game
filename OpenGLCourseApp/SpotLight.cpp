#include "SpotLight.h"

SpotLight::SpotLight() : PointLight(), 
						direction{ glm::vec3(0.0f, -1.0f, 0.0f) }, edge{ 0.0f }, procEdge{ cos(glm::radians(edge)) }, isOn{ true } { }

SpotLight::SpotLight(GLuint shadowWidth, GLuint shadowHeight,		// shadow map texture width and height
						GLfloat near, GLfloat far,                  // light's "camera" near and far plane
						GLfloat red, GLfloat green, GLfloat blue,   // Inherit colors from Light class
						GLfloat aIntensity, GLfloat dIntensity,     // light intensity (ambient and directional)
						GLfloat xPos, GLfloat yPos, GLfloat zPos,   // light position
						GLfloat xDir, GLfloat yDir, GLfloat zDir,   // direction of light rays
						GLfloat con, GLfloat lin, GLfloat exp,      // attenuation values
						GLfloat edge) :                             // angle of light boundary 
							PointLight(shadowWidth, shadowHeight, near, far, red, green, blue, 
								aIntensity, dIntensity, xPos, yPos, zPos, con, lin, exp),
								direction{ glm::normalize(glm::vec3(xDir, yDir, zDir)) }, 
								edge {edge}, procEdge{cosf(glm::radians(edge))} { }

void SpotLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
							GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionLocation,
							GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation,
							GLuint edgeLocation)
{
	glUniform3f(ambientColorLocation, color.x, color.y, color.z);

	if (isOn)
	{
		glUniform1f(ambientIntensityLocation, ambientIntensity);
		glUniform1f(diffuseIntensityLocation, diffuseIntensity);
	}
	else
	{
		glUniform1f(ambientIntensityLocation, 0.0f);
		glUniform1f(diffuseIntensityLocation, 0.0f);
	}

	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentLocation, exponent);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(edgeLocation, procEdge);
}


void SpotLight::SetFlash(glm::vec3 pos, glm::vec3 dir) // set spotlight to position and direction
{
	position = pos;
	direction = dir;
}

SpotLight::~SpotLight() { }
