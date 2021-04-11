#include "Light.h"

Light::Light()
{
	color = glm::vec3(1.0f, 1.0f, 1.0f); // how much of each light value will be shown, 0.0 - 1.0
	ambientIntensity = 1.0f;
	diffuseIntensity = 0.0f;
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity)
{
	color = glm::vec3(red, green, blue); // how much of each light value will be shown, 0.0 - 1.0
	ambientIntensity = aIntensity;
	diffuseIntensity = dIntensity;
}

Light::~Light()
{

}
