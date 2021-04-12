#include "Shader.h"

// only default constructor exists
Shader::Shader() : pointLightCount{ 0 }, shaderID{ 0 }, uniformProjection{ 0 }, uniformModel{ 0 }, uniformView{ 0 },
					uniformEyePosition{ 0 }, uniformSpecularIntensity{ 0 }, uniformShininess{ 0 }, uniformPointLightCount{ 0 }
{ 
}


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

	// Validate shader program
	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error validating  program: '%s'\n", eLog);
		return;
	}
	// End error handling
	

	// Assign ids to shader uniforms. Essentially creates aliases for each uniform that can be called outside shader code
	uniformModel = glGetUniformLocation(shaderID, "model");
	uniformProjection = glGetUniformLocation(shaderID, "projection");
	uniformView = glGetUniformLocation(shaderID, "view");
	uniformDirectionalLight.uniformColor = glGetUniformLocation(shaderID, "directionalLight.base.color");
	uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.baseambientIntensity");
	uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction");
	uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.base.diffuseIntensity");
	uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
	uniformShininess = glGetUniformLocation(shaderID, "material.shininess");
	uniformEyePosition = glGetUniformLocation(shaderID, "eyePosition");

	uniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount");

	// Assign uniform IDs for each point light
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

#pragma endregion


// Define shader in source code. Not currently used
void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
	CompileShader(vertexCode, fragmentCode);
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

#pragma endregion


// Pass applicable uniform variables to DirectionLight object to set those values
void Shader::SetDirectionalLight(DirectionalLight& dLight)
{
	dLight.UseLight(uniformDirectionalLight.uniformAmbientIntensity, uniformDirectionalLight.uniformColor,
		uniformDirectionalLight.uniformDiffuseIntensity, uniformDirectionalLight.uniformDirection);
}


// Pass applicable uniform variables to array of PointLight objects to set the values of each
void Shader::SetPointLights(PointLight* pLight, unsigned int lightCount)
{
	if (lightCount > MAX_POINT_LIGHTS) 
		lightCount = MAX_POINT_LIGHTS;	

	glUniform1i(uniformPointLightCount, lightCount);

	for (size_t i = 0; i < lightCount; i++)
	{
		pLight[i].UseLight(uniformPointLight[i].uniformAmbientIntensity, uniformPointLight[i].uniformColor,
			uniformPointLight[i].uniformDiffuseIntensity, uniformPointLight[i].uniformPosition,
			uniformPointLight[i].uniformConstant, uniformPointLight[i].uniformLinear, uniformPointLight[i].uniformExponent);
	}
}

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
