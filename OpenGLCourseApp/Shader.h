#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL\glew.h>

#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "CommonValues.h"

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

class Shader
{
public:
	Shader();

	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);
	void CreateFromFiles(const char* vertexLocation, const char* geometryLocation, const char* fragmentLocation);

	void Validate(); // note: no args because values stored in shader itself

	std::string ReadFile(const char* fileLocation);

	// Shader Location Getters
	GLuint GetProjectionLocation();
	GLuint GetModelLocation();
	GLuint GetViewLocation();
	GLuint GetAmbientIntensityLocation();
	GLuint GetAmbientColorLocation();
	GLuint GetDiffuseIntensityLocation();
	GLuint GetDirectionLocation();
	GLuint GetSpecularIntensityLocation();
	GLuint GetShininessLocation();
	GLuint GetEyePositionLocation();
	GLuint GetOmniLightPosLocation();
	GLuint GetFarPlaneLocation();

	// Pass shader uniform variables to applicable objects for Setting
	void SetDirectionalLight(DirectionalLight& dLight);
	void SetPointLights(PointLight* pLight, unsigned int lightCount, 
							unsigned int textureUnit, // starting texture unit
							unsigned int offset);     // offset is the offset in omniShadowMap struct array in shader.frag 
	void SetSpotLights(SpotLight* sLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset);
	void SetTexture(GLuint textureUnit);								// which texture unit to bind to the uniform value for texture
	void SetDirectionalShadowMap(GLuint textureUnit);					// which texture to bind to directional shadow map
	void SetDirectionalLightTransform(glm::mat4* lTransform);
	void SetLightMatrices(std::vector<glm::mat4> lightMatrices); 
	
	void UseShader();
	void ClearShader();

	~Shader();

private:
	int pointLightCount;	// number of point lights
	int spotLightCount;		// number of spot lights

	GLuint shaderID, uniformProjection, uniformModel, uniformView, uniformEyePosition,  // matrix IDs
		uniformSpecularIntensity, uniformShininess,										// material IDs
		uniformTexture,																	// texture IDs
		uniformDirectionalLightTransform, uniformDirectionalShadowMap,					// shadow IDs
		uniformOmniLightPos, uniformFarPlane;											// IDs for omni_shadow_map.frag

	GLuint uniformLightMatrices[6];			// IDs for omni_shadow_map.geom

	GLuint uniformPointLightCount;			// ID of point light count
	GLuint uniformSpotLightCount;			// ID of spot light count

	struct // can contain multiple uniform locations for different directional lights	// IDs of Directional Light struct members
	{
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformDirection;
	} uniformDirectionalLight;

	struct // can contain multiple uniform locations for point lights					// IDs of Point Light struct members
	{
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;
	} uniformPointLight[kMaxPointLights];

	struct																				// IDs of Spot Light struct members
	{
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;

		GLuint uniformDirection;
		GLuint uniformEdge;
	} uniformSpotLight[kMaxSpotLights];

	struct // corresponds to OmniShadowMap struct in shader.frag
	{
		GLuint shadowMap;
		GLuint farPlane;
	} uniformOmniShadowMap[kMaxPointLights + kMaxSpotLights];

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void CompileShader(const char* vertexCode, const char* geometryCode, const char* fragmeentCode);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);

	// helper for CompileShaders
	void CompileProgram();
};

