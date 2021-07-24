/*
*	Light object with attributes for color, intensity, position of light source, and attenuation factor
*	This light has a position and shines in all directions. e.g. a naked light bulb
*/

#pragma once
#include "Light.h"
#include "OmniShadowMap.h"

#include <vector>

class PointLight :
    public Light
{
public:
    PointLight();

    PointLight(GLuint shadowWidth, GLuint shadowHeight, // shadow map texture width and height 
        GLfloat near, GLfloat far,                      // light's "camera" near and far planes
        GLfloat red, GLfloat green, GLfloat blue,       // Inherit colors from Light class
        GLfloat aIntensity, GLfloat dIntensity,         // Inherit intensities from Light class
        GLfloat xPos, GLfloat yPos, GLfloat zPos,       // Position of light source. Unique to DirectionalLight
        GLfloat con, GLfloat lin, GLfloat exp);         // Attenuation factors. Determines strength of light over distance

	// Set uniform variables related to pointLight. Called from Shader.SetPointLight
    void UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
        GLuint diffuseIntensityLocation, GLuint positionLocation,
        GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation);

    std::vector<glm::mat4> CalculateLightTransform();   // calculate matrices for each cube map side

    GLfloat GetFarPlane();              // for attaching to omnishadowmap fragment shader
    glm::vec3 GetPosition();

    ~PointLight();

protected:
    glm::vec3 position;                 // light position

    GLfloat constant, linear, exponent; // attenuation quadratic values, for distance = x, ax^2 + bx + c

    GLfloat farPlane;                   // distance light projects
};

