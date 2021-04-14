/*
*	Light object with attributes for color, intensity, position of light source, and attenuation factor
*	This light has a position and shines in all directions. e.g. a naked light bulb
*/

#pragma once
#include "Light.h"

class PointLight :
    public Light
{
public:
    PointLight();

    PointLight(GLfloat red, GLfloat green, GLfloat blue, // Inherit colors from Light class
        GLfloat aIntensity, GLfloat dIntensity,          // Inherit intensities from Light class
        GLfloat xPos, GLfloat yPos, GLfloat zPos,        // Position of light source. Unique to DirectionalLight
        GLfloat con, GLfloat lin, GLfloat exp);          // Attenuation factors. Determines strength of light over distance

	// Set uniform variables related to pointLight. Called from Shader.SetPointLight
    void UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
        GLuint diffuseIntensityLocation, GLuint positionLocation,
        GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation);

    ~PointLight();

protected:
    glm::vec3 position;                 // light position

    GLfloat constant, linear, exponent; // attenuation quadratic values, for distance = x, ax^2 + bx + c
};

