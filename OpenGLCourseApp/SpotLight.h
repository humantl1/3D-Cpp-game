#pragma once
#include "PointLight.h"

class SpotLight :
    public PointLight
{
public:
    SpotLight();

    SpotLight(GLfloat red, GLfloat green, GLfloat blue, 
		GLfloat aIntensity, GLfloat dIntensity,     // light intensity (ambient and directional)
        GLfloat xPos, GLfloat yPos, GLfloat zPos,   // light position
        GLfloat xdir, GLfloat yDir, GLfloat zDir,   // direction of light rays
		GLfloat con, GLfloat lin, GLfloat exp,      // attenuation values
        GLfloat edge);                              // angle of light boundary 

    void UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
        GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionLocation,
        GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation,
        GLuint edgeLocation);

    void SetFlash(glm::vec3 pos, glm::vec3 dir); // set spotlight to position and direction

    ~SpotLight();

private:
    glm::vec3 direction; // direction light is pointing
    GLfloat edge, procEdge; // angle of light boundary (raw value , cos in radians)
};

