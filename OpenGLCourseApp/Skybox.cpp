#include "Skybox.h"

Skybox::Skybox() {};

/* Skybox Construction Procedure
 * 1. Create skybox shader from text file
 * 2. Generate and bind texture object
 * 3. Loop through each cube map layer (6 faces):
 *		a. Load image into memory
 *		b. Check image is valid
 *		c. Pass image to shader as a cube map layer
 *		d. Free image from memory
 * 4. Set skybox edge and mipmap parameters
 * 5. Set indices and vertices for skybox mesh
 * 6. Create skybox mesh
 */
Skybox::Skybox(std::vector<std::string> faceLocations) :
	skyShader{ new Shader() }, // location of each skybox cube map face 
	skyMesh {new Mesh() }
{
	// Shader Setup
	skyShader->CreateFromFiles("Shaders/skybox.vert", "Shaders/skybox.frag");

	uniformProjection = skyShader->GetProjectionLocation();
	uniformView = skyShader->GetViewLocation();

	// Texture Setup
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, bitDepth;

	for (size_t i = 0; i < 6; i++)
	{
		// load image
		unsigned char* texData = stbi_load(faceLocations[i].c_str(), &width, &height, &bitDepth, 0);

		// Error checking for valid texture data
		if (!texData)
		{
			printf("Failed to find: %s\n", faceLocations[i].c_str());
			return;
		}

		// Make textures readable by shader (for each face)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
		stbi_image_free(texData); // free texture from memory, since it has already been passed to shader
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);		
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);			// set mipmap minifying function to linear
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);			// set mipmap magnifying function to linear

	// Mesh Setup
	unsigned int skyboxIndices[] = {
		//front
		0, 1, 2,
		2, 1, 3,
		//right
		2, 3, 5,
		5, 3, 7,
		//back
		5, 7, 4,
		4, 7, 6,
		//left
		4, 6, 0,
		0, 6, 1,
		//top
		4, 0, 5,
		5, 0, 2,
		//bottom
		1, 6, 3,
		3, 6, 7
	};

	float skyboxVertices[] = {
		// "back" vertices		//uv values unnecessary, as vector will point to texture coordinates //normals uneccesary as lighting is N/A
		-1.0f, 1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,

		// "front" vertices
		-1.0f, 1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		-1.0f, -1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f
	};

//	skyMesh = new Mesh();
	skyMesh->CreateMesh(skyboxVertices, skyboxIndices, 64, 36);
}


/* Draw skybox procedure
 * 0. Remove transformations by converting to 3d matrix then back to 4d
 * 1. Turn off depth mask so skybox can be drawn "through"
 * 2. Set skybox shader to as active shader
 * 3. Pass projection and camera matrices to shader
 * 4. Set as texture unit 0 as activeactive texture unit 0

 * 5. Bind textureID to cube map
 * 6. Check skybox shader is valid
 * 7. Draw the skybox
 * 8. Turn depth mask back on!
 */
void Skybox::DrawSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
	viewMatrix = glm::mat4(glm::mat3(viewMatrix)); // cut off transformations to keep skybox centered on camera

	glDepthMask(GL_FALSE);  // allow skybox to be drawn through

	skyShader->UseShader(); // ensure skybox shader is the active shader

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix)); // pass projection matrix to shader program
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));			  // pass camera matrix to shader program

	glActiveTexture(GL_TEXTURE0); // active texture can be 0 because skybox ONLY uses texture cube map and does not interact with other shaders
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	skyShader->Validate(); // error checking

	skyMesh->RenderMesh(); // draw the skybox!

	glDepthMask(GL_TRUE);
}

Skybox::~Skybox() {}
