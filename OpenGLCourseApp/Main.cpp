#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"

// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;
const float toRadians = 3.14159265f / 180.0f;

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
		-1.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 12, 12);
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
	// Initialize GLFW
	if (!glfwInit())
	{
		printf("GLFW Initialization failed!");
		glfwTerminate();
		return 1;
	}

	// Setup GLFW window properties
	// OpenGL version (3.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Core profile = No Backwards Compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow forward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//Create GLFW window
	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
	if (!mainWindow)
	{
		printf("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	// Get Buffer size information
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	// Allow modern extension features
	glewExperimental = GL_TRUE;

	// glewInit returns an enum GLEW_OK if initialized
	if (glewInit() != GLEW_OK)
	{
		printf("GLEW initialization failed!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}
	
	glEnable(GL_DEPTH_TEST); // this tests the depth of each pixel to see if it should be drawn

	// create Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);

	CreateObjects();
	CreateShaders();

	GLuint uniformProjection = 0, uniformModel = 0;

	glm::mat4 projection = glm::perspective(
		45.0f, // fov from top to bottom
		(GLfloat)bufferWidth / (GLfloat)bufferHeight, // aspect ratio
		0.1f, // near z
		100.0f); // far z

	// Loop until window closed
	while (!glfwWindowShouldClose(mainWindow))
	{
		// Get + handle user input events
		glfwPollEvents();

		// Clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the color and depth buffers

		shaderList[0].UseShader(); // start using the shader program
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();

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
			meshList[0]->RenderMesh();
			
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f)); // glm function builds matrix to translate in direction and magnitude of vector
			model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f)); // (matrix, (scale coordinates))
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			meshList[1]->RenderMesh();

		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
	}

	return 0;
}