#pragma once
#include "Light.h"

class PointLight :
    public Light
{
public:
    PointLight();

    PointLight(GLfloat red, GLfloat green, GLfloat blue, 
		GLfloat aIntensity, GLfloat dIntensity,     // light intensity (ambient and directional)
        GLfloat xPos, GLfloat yPos, GLfloat zPos,   // light position
		GLfloat con, GLfloat lin, GLfloat exp);     // attenuation values

    void UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
        GLuint diffuseIntensityLocation, GLuint positionLocation,
        GLfloat constantLocation, GLfloat linearLocation, GLfloat exponentLocation);

    ~PointLight();

private:
    glm::vec3 position;                 // light position

    GLfloat constant, linear, exponent; // attenuation quadratic values, for distance = x, ax^2 + bx + c
};

