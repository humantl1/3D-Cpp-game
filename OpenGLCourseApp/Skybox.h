#pragma once

#include <vector>
#include <string>

#include <GL\glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CommonValues.h"

#include "Mesh.h"
#include "Shader.h"

class Skybox
{
public:
	Skybox();
	Skybox(std::vector<std::string> faceLocations); // location of each skybox cube map face

	void DrawSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);


	~Skybox();

private:
	Mesh* skyMesh;
	Shader* skyShader;

	GLuint textureID;						// ID for skybox texture
	GLuint uniformProjection, uniformView;  // Uniform IDs for skybox matrices (will be same for any skybox)
};

