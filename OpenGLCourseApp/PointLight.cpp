#include "PointLight.h"

PointLight::PointLight() : Light(), 
							position{ glm::vec3(0.0f, 0.0f, 0.0f) },
							constant{ 1.0f }, linear{ 0.0f }, exponent{ 0.0f } { }

PointLight::PointLight(GLuint shadowWidth, GLuint shadowHeight,						// shadow map texture width and height 
						GLfloat near, GLfloat far,									// light's "camera" near and far planes
						GLfloat red, GLfloat green, GLfloat blue,					// Inherit colors from Light class
						GLfloat aIntensity, GLfloat dIntensity,						// Inherit intensities from Light class
						GLfloat xPos, GLfloat yPos, GLfloat zPos,					// Position of light source. Unique to DirectionalLight
						GLfloat con, GLfloat lin, GLfloat exp) :					// Attenuation factors. Determines strength of light over distance
							Light(shadowWidth, shadowHeight,						// shadow map width and height 
								red, green, blue, aIntensity, dIntensity),			// Position and attenuation are unique to PointLights
								farPlane{far}, position{ glm::vec3(xPos, yPos, zPos) },
								constant {con}, linear {lin}, exponent {exp} 
{ 
	float aspect = (float)shadowWidth / (float)shadowHeight;						// for a cube map, aspect should = 1
	lightProj = glm::perspective(glm::radians(90.0f), aspect, near, far);			// only one projection type is needed for the six views

	// create omnishadow map (cube map) for this light source
	shadowMap = new OmniShadowMap();
	shadowMap->Init(shadowWidth, shadowHeight);
}

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

// distance light projects
GLfloat PointLight::GetFarPlane()
{
	return farPlane;
}

glm::vec3 PointLight::GetPosition()
{
	return position;
}

// create a light projection matrix for each face of the cube map
std::vector<glm::mat4> PointLight::CalculateLightTransform()
{
	std::vector<glm::mat4> lightMatrices;
	lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));		// +x
	lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));	// -x
	lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));      // +y
	lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));    // -y
	lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));		// +z
	lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));	// -z

	return lightMatrices;
}


PointLight::~PointLight()
{
}
