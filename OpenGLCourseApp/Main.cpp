#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
Camera camera;

Texture rockTexture;
Texture asteroidTexture;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

static const char* vShader = "Shaders/shader.vert";
static const char* fShader = "Shaders/shader.frag";

void CreateObjects()
{
	// pyramid faces composed of vertex indices
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 3
	};

	GLfloat vertices[] = {
		// x     y     z		 u     v
		-1.0f, -1.0f, 0.0f,		0.0f, 0.0f,		// bottom left
		0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		// "back"
		1.0f, -1.0f, 0.0f,		1.0f, 0.0f,		// bottom right
		0.0f, 1.0f, 0.0f,		0.5f, 1.0f		// top 
	};

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 20, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 20, 12);
	meshList.push_back(obj2);
} 

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main()
{
	mainWindow = Window(800, 600);
	mainWindow.initialize();
	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.00f, 0.2f);

 	rockTexture = Texture((char*)"Textures/rock.png");
	rockTexture.LoadTexture();
	asteroidTexture = Texture((char*)"Textures/asteroid.png");
	asteroidTexture.LoadTexture();

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;

	glm::mat4 projection = glm::perspective(
		glm::radians(45.0f), // fov from top to bottom
		mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight(), // aspect ratio
		0.1f, // near z
		100.0f); // far z

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime(); // returns time in seconds
		deltaTime = now - lastTime;
		lastTime = now;

		// Get + handle user input events
		glfwPollEvents();

		camera.keyControl(mainWindow.getKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the color and depth buffers

		shaderList[0].UseShader(); // start using the shader program
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();

			// Initialize transformation matrix from triOffset. The next three line are just GLM, they aren't openGL. 
			// The order of transformations is essentialy backwards
			glm::mat4 model(1.0f); // initialize simple 4x4 identity matrix using GLM
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f)); // glm function builds matrix to translate in direction and magnitude of vector
			//model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); // (matrix, angle of rotation, axis of rotation). This must concatenate the previous matrix
			model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f)); // (matrix, (scale coordinates))

			// pass the model matrix initialized above to shader program
			glUniformMatrix4fv(uniformModel, // location of matrix in shader
				1, // number of matrices to pass(?)
				GL_FALSE, // transpose matrix?
				glm::value_ptr(model)); // matrix to pass to shader matrix

			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection)); // pass projection matrix to shader program
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix())); // pass camera matrix to shader program
			rockTexture.UseTexture();
			meshList[0]->RenderMesh();
			
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f)); // glm function builds matrix to translate in direction and magnitude of vector
			model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f)); // (matrix, (scale coordinates))
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			asteroidTexture.UseTexture();
			meshList[1]->RenderMesh();

		glUseProgram(0);

		mainWindow.swapBuffer();
	}

	return 0;
}