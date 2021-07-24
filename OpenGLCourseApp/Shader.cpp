#include "Shader.h"

// only default constructor exists
Shader::Shader() : shaderID{ 0 }, uniformProjection{ 0 }, uniformModel{ 0 }, uniformView{ 0 },
					uniformEyePosition{ 0 }, uniformSpecularIntensity{ 0 }, uniformShininess{ 0 }, uniformPointLightCount{ 0 },
					pointLightCount{ 0 }, spotLightCount{ 0 } { }

#pragma region Create Shader from File

//  Get, link, and compile shaders from file.
void Shader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation)
{
	std::string vertexString = ReadFile(vertexLocation);
	std::string fragmentString = ReadFile(fragmentLocation);
	const char* vertexCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	CompileShader(vertexCode, fragmentCode);
}

//  Get, link, and compile shaders from file. For use with 3 file omnidirectional shadow shaders
void Shader::CreateFromFiles(const char* vertexLocation, const char* geometryLocation, const char* fragmentLocation)
{
	std::string vertexString = ReadFile(vertexLocation);
	std::string geometryString = ReadFile(geometryLocation);
	std::string fragmentString = ReadFile(fragmentLocation);
	const char* vertexCode = vertexString.c_str();
	const char* geometryCode = geometryString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	CompileShader(vertexCode, geometryCode, fragmentCode);
}

// Get shader text file as single string
std::string Shader::ReadFile(const char* fileLocation)
{
	std::string content;
	std::ifstream fileStream(fileLocation, std::ios::in);

	if (!fileStream.is_open())
	{
		printf("Failed to read %s! File doesn't exist.", fileLocation);
		return "";
	}

	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}
	fileStream.close();
	return content;
}

// compile 2 part shaders
void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
	// create empty program object / assign 
	shaderID = glCreateProgram();

	if (!shaderID)
	{
		printf("Shader ID invalid!\n");
		return;
	}


	// create shaders and attach to shader program
	AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);


	// Link shaders attached to program object. Creates executables for each shader type
	glLinkProgram(shaderID);

	CompileProgram();
}

// compile 2 part (omnishadowmap shaders)
void Shader::CompileShader(const char* vertexCode, const char* geometryCode, const char* fragmentCode)
{
	// create empty program object / assign 
	shaderID = glCreateProgram();

	if (!shaderID)
	{
		printf("Shader ID invalid!\n");
		return;
	}


	// create shaders and attach to shader program
	AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderID, geometryCode, GL_GEOMETRY_SHADER);
	AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);


	// Link shaders attached to program object. Creates executables for each shader type
	glLinkProgram(shaderID);

	CompileProgram();
}

#pragma endregion


// Define shader in source code. Not currently used
void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
	CompileShader(vertexCode, fragmentCode);
}

void Shader::Validate()
{	
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	// Validate shader program
	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error validating  program: '%s'\n", eLog);
		return;
	}
}

// handles shader validation and uniform ID assignments
void Shader::CompileProgram()
{
// Error handling
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	// Check link status
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}


	// Assign IDs to shader uniforms. Essentially creates aliases for each uniform that can be called outside shader code
	uniformModel = glGetUniformLocation(shaderID, "model");
	uniformProjection = glGetUniformLocation(shaderID, "projection");
	uniformView = glGetUniformLocation(shaderID, "view");
	uniformDirectionalLight.uniformColor = glGetUniformLocation(shaderID, "directionalLight.base.color");
	uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.base.ambientIntensity");
	uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction");
	uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.base.diffuseIntensity");
	uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
	uniformShininess = glGetUniformLocation(shaderID, "material.shininess");
	uniformEyePosition = glGetUniformLocation(shaderID, "eyePosition");


	// Point light uniforms IDs
	uniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount");

	// Uniform IDs in case of multiple point lights
	for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		char locBuff[100] = { '\0' };

		// snprintf prints to buffer. Int this case, locBuff
		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.color", i);
		uniformPointLight[i].uniformColor = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.ambientIntensity", i);
		uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.diffuseIntensity", i);
		uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].position", i);
		uniformPointLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].constant", i);
		uniformPointLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].linear", i);
		uniformPointLight[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].exponent", i);
		uniformPointLight[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);
	}


	// Spot light uniforms IDs
	uniformSpotLightCount = glGetUniformLocation(shaderID, "spotLightCount");

	// Uniform IDs in case of multiple spot lights
	for (size_t i = 0; i < MAX_SPOT_LIGHTS; i++)
	{
		char locBuff[100] = { '\0' };

		// snprintf prints to buffer. Int this case, locBuff
		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.color", i);
		uniformSpotLight[i].uniformColor = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.ambientIntensity", i);
		uniformSpotLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.diffuseIntensity", i);
		uniformSpotLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.position", i);
		uniformSpotLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.constant", i);
		uniformSpotLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.linear", i);
		uniformSpotLight[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.exponent", i);
		uniformSpotLight[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].direction", i);
		uniformSpotLight[i].uniformDirection = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].edge", i);
		uniformSpotLight[i].uniformEdge = glGetUniformLocation(shaderID, locBuff);
	}

	// set IDs for shadows
	uniformTexture = glGetUniformLocation(shaderID, "theTexture");
	uniformDirectionalLightTransform = glGetUniformLocation(shaderID, "directionalLightTransform");
	uniformDirectionalShadowMap = glGetUniformLocation(shaderID, "directionalShadowMap");

	// set IDs for omni_shadow_map.frag
	uniformOmniLightPos = glGetUniformLocation(shaderID, "lightPos");
	uniformFarPlane = glGetUniformLocation(shaderID, "farPlane");

	// Set IDs for array of Light Matrices in omni_shadow_map.geom
	for (size_t i = 0; i < 6; i++)
	{
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "lightMatrices[%d]", i);
		uniformLightMatrices[i] = glGetUniformLocation(shaderID, locBuff);
	}

	// Set IDs for omniShadowMaps struct members in shader.frag
	for (size_t i = 0; i < MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS; i++)
	{
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "omniShadowMaps[%d].shadowMap", i);
		uniformOmniShadowMap[i].shadowMap = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "omniShadowMaps[%d].farPlane", i);
		uniformOmniShadowMap[i].farPlane = glGetUniformLocation(shaderID, locBuff);
	}

}

// compile shader(s) and attach to shader program
void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	// create empty shader object (to be attached to shader program), return handle
	GLuint theShader = glCreateShader(shaderType);


	// prepare code for insertion into shader
	const GLchar* theCode[1]; // an array because more than one shader can exist
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode); // length of code in shader


	// store (and replace) shader code in shader object then compile
	glShaderSource(theShader,	// handle of shader object
		1,						// number of strings in code array (using a single big string here)
		theCode,				// code to transfer into shader object
		codeLength);			// array containing the length of each string in the code array 
	// Note: source code is now copied, so code string is now redundant
	glCompileShader(theShader); // compile code in the shader


	// shader initialization error handling
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(theShader, sizeof(eLog), NULL, eLog);
		printf("Error compiling the %d program: '%s'\n", shaderType, eLog);
		return;
	}
	// end error handling


	// attach compiled shader to shader program
	glAttachShader(theProgram, theShader);

}

#pragma region Getters

// Get handles of shader uniform variables

GLuint Shader::GetProjectionLocation()
{
	return uniformProjection;
}

GLuint Shader::GetModelLocation()
{
	return uniformModel;
}

GLuint Shader::GetViewLocation()
{
	return uniformView;
}

GLuint Shader::GetAmbientColorLocation()
{
	return uniformDirectionalLight.uniformColor;
}

GLuint Shader::GetAmbientIntensityLocation()
{
	return uniformDirectionalLight.uniformAmbientIntensity;
}

GLuint Shader::GetDiffuseIntensityLocation()
{
	return uniformDirectionalLight.uniformDiffuseIntensity;
}

GLuint Shader::GetDirectionLocation()
{
	return uniformDirectionalLight.uniformDirection;
}

GLuint Shader::GetSpecularIntensityLocation()
{
	return uniformSpecularIntensity;
}

GLuint Shader::GetShininessLocation()
{
	return uniformShininess;
}

GLuint Shader::GetEyePositionLocation()
{
	return uniformEyePosition;
}

GLuint Shader::GetOmniLightPosLocation()
{
	return uniformOmniLightPos;
}

GLuint Shader::GetFarPlaneLocation()
{
	return uniformFarPlane;
}

#pragma endregion

#pragma region Setters

// Pass applicable uniform variables to DirectionLight object to set those values
void Shader::SetDirectionalLight(DirectionalLight& dLight)
{
	dLight.UseLight(uniformDirectionalLight.uniformAmbientIntensity, uniformDirectionalLight.uniformColor,
		uniformDirectionalLight.uniformDiffuseIntensity, uniformDirectionalLight.uniformDirection);
}


// Pass applicable uniform variables to array of PointLight objects to set the values of each in shader.frag
void Shader::SetPointLights(PointLight* pLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset)

{
	if (lightCount > MAX_POINT_LIGHTS) 
		lightCount = MAX_POINT_LIGHTS;	

	glUniform1i(uniformPointLightCount, lightCount);

	for (size_t i = 0; i < lightCount; i++)
	{
		pLight[i].UseLight(uniformPointLight[i].uniformAmbientIntensity, uniformPointLight[i].uniformColor,
			uniformPointLight[i].uniformDiffuseIntensity, uniformPointLight[i].uniformPosition,
			uniformPointLight[i].uniformConstant, uniformPointLight[i].uniformLinear, uniformPointLight[i].uniformExponent);

		// shadow data
		pLight[i].GetShadowMap()->Read(GL_TEXTURE0 + textureUnit + i);					 // pass correct shadow map texture
		glUniform1i(uniformOmniShadowMap[i + offset].shadowMap, textureUnit + i);		 // nonzero offset is optional
		glUniform1f(uniformOmniShadowMap[i + offset].farPlane, pLight[i].GetFarPlane()); // nonzero offset is optional
	}
}

// Pass applicable uniform variables to array of SpotLight objects to set the values of each in shader.frag
void Shader::SetSpotLights(SpotLight* sLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset)
{
	if (lightCount > MAX_SPOT_LIGHTS) 
		lightCount = MAX_SPOT_LIGHTS;	

	glUniform1i(uniformSpotLightCount, lightCount);

	for (size_t i = 0; i < lightCount; i++)
	{
		sLight[i].UseLight(uniformSpotLight[i].uniformAmbientIntensity, uniformSpotLight[i].uniformColor,
			uniformSpotLight[i].uniformDiffuseIntensity, uniformSpotLight[i].uniformPosition, uniformSpotLight[i].uniformDirection,
			uniformSpotLight[i].uniformConstant, uniformSpotLight[i].uniformLinear, uniformSpotLight[i].uniformExponent,
			uniformSpotLight[i].uniformEdge);

		// shadow data
		sLight[i].GetShadowMap()->Read(GL_TEXTURE0 + textureUnit + i);					 // pass correct shadow map texture
		glUniform1i(uniformOmniShadowMap[i + offset].shadowMap, textureUnit + i);		 // nonzero offset is optional
		glUniform1f(uniformOmniShadowMap[i + offset].farPlane, sLight[i].GetFarPlane()); // nonzero offset is optional

	}

}

// Set shadow uniforms:
void Shader::SetTexture(GLuint textureUnit)
{
	glUniform1i(uniformTexture, textureUnit);
}

void Shader::SetDirectionalShadowMap(GLuint textureUnit)
{
	glUniform1i(uniformDirectionalShadowMap, textureUnit);
}
 
void Shader::SetDirectionalLightTransform(glm::mat4* lTransform)
{
	// args: GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
	glUniformMatrix4fv(uniformDirectionalLightTransform, 1, GL_FALSE, glm::value_ptr(*lTransform)); // not sure why it's necessary to dereference a pointer to create a pointer to the dereference value
}

void Shader::SetLightMatrices(std::vector<glm::mat4> lightMatrices)
{
	for (size_t i = 0; i < 6; i++)
	{
		// args: GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
		glUniformMatrix4fv(uniformLightMatrices[i], 1, GL_FALSE, glm::value_ptr(lightMatrices[i]));
	}
}

#pragma endregion

// sets instance of shader to active shader
void Shader::UseShader()
{
	glUseProgram(shaderID);
}

void Shader::ClearShader()
{
	if (shaderID != 0)
	{
		glDeleteProgram(shaderID);
		shaderID = 0;
	}

	uniformModel = 0;
	uniformProjection = 0;
}


Shader::~Shader()
{
	ClearShader();
}
