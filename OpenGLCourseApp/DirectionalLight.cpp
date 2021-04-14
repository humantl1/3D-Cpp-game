#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light(), 
										direction {glm::vec3(0.0f, -1.0f, 0.0f)} { }

DirectionalLight::DirectionalLight(GLfloat red, GLfloat green, GLfloat blue,				// Inherit colors from Light class
									GLfloat aIntensity, GLfloat dIntensity,					// Inherit intensities from Light class
									GLfloat xDir, GLfloat yDir, GLfloat zDir) :				// Direction light is shining. Unique to DirectionalLight
										Light(red, green, blue, aIntensity, dIntensity)	    // Only directional component is different then base Light
{
	direction = glm::vec3(xDir, yDir, zDir);
}

// Set uniform variables related to directional light. Called from Shader.SetDirectionalLight
void DirectionalLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
		GLuint diffuseIntensityLocation, GLuint directionLocation)
{
	glUniform3f(ambientColorLocation, color.x, color.y, color.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}
DirectionalLight::~DirectionalLight()
{

}
