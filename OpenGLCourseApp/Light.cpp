#include "Light.h"

Light::Light() //: color {glm::vec3(1.0f, 1.0f, 1.0f)}, 
				//	ambientIntensity {1.0f},	// Ambient intensity illuminates everything equally 
				//	diffuseIntensity{0.0f} { }	// Diffuse intensity illuminates based on angle of face
{
	color = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientIntensity = 0.0f;
	diffuseIntensity = 0.0f;
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity)// :
					//color {glm::vec3(red, green, blue)},
				//	ambientIntensity {aIntensity},
				//	diffuseIntensity {dIntensity} { }
{
	color = glm::vec3(red, green, blue);
	ambientIntensity = aIntensity;
	diffuseIntensity = dIntensity;
}

Light::~Light()
{

}
