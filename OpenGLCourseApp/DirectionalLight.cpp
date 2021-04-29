#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light(), 
										direction {glm::vec3(0.0f, -1.0f, 0.0f)} 
{ 
	lightProj = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f);
}

DirectionalLight::DirectionalLight(GLfloat shadowWidth, GLfloat shadowHeight,				// shadow map resolution
									GLfloat red, GLfloat green, GLfloat blue,				// Inherit colors from Light class
									GLfloat aIntensity, GLfloat dIntensity,					// Inherit intensities from Light class
									GLfloat xDir, GLfloat yDir, GLfloat zDir) :				// Direction light is shining. Unique to DirectionalLight
										Light(shadowWidth, shadowHeight, red, green, blue, aIntensity, dIntensity) // Only directional component is different then base Light
{
	direction = glm::vec3(xDir, yDir, zDir);
	lightProj = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f); // light frustum: left, right, bottom, top, near, far
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

// return projection and view combined
glm::mat4 DirectionalLight::CalculateLightTransform()
{
	return lightProj * glm::lookAt(-direction, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // lookAt args: position, target, up vector
	// TODO: if we are looking straight up or down, the last arg of lookAt should be glm::vec3(0.0f, 1.0f, 1.0f)
	// could create a check for that and set appropriately
}

DirectionalLight::~DirectionalLight()
{

}
