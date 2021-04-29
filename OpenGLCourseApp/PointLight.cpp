#include "PointLight.h"

PointLight::PointLight() : Light(), 
							position{ glm::vec3(0.0f, 0.0f, 0.0f) },
							constant{ 1.0f }, linear{ 0.0f }, exponent{ 0.0f } { }

PointLight::PointLight(GLfloat red, GLfloat green, GLfloat blue,					// Inherit colors from Light class
						GLfloat aIntensity, GLfloat dIntensity,						// Inherit intensities from Light class
						GLfloat xPos, GLfloat yPos, GLfloat zPos,					// Position of light source. Unique to DirectionalLight
						GLfloat con, GLfloat lin, GLfloat exp) :					// Attenuation factors. Determines strength of light over distance
							Light(1024, 1024,										// shadow map width and height (temp values)
								red, green, blue, aIntensity, dIntensity),			// Position and attenuation are unique to PointLights
								position {glm::vec3(xPos, yPos, zPos)},
								constant {con}, linear {lin}, exponent {exp} { }

// Set uniform variables related to pointLight. Called from Shader.SetPointLight
void PointLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation, 
	GLuint diffuseIntensityLocation, GLuint positionLocation, 
	GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation)
{
	glUniform3f(ambientColorLocation, color.x, color.y, color.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);

	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentLocation, exponent);
}

PointLight::~PointLight()
{
}
