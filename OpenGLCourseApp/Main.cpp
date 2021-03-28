#include <stdio.h>
#include <string.h>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;
const float toRadians = 3.14159265f / 180.0f;

GLuint VAO, VBO, IBO, shader, uniformModel;

// movement variables
bool direction = true; // true for right
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.005f;

// rotation variables
float curAngle = 0.0f;

// scaling variables
bool sizeDirection = true;
float curSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;

// Vertex Shader
static const char* vShader = "												\n\
#version 330																\n\
																			\n\
layout (location = 0) in vec3 pos;											\n\
																			\n\
out vec4 vColor;															\n\
																			\n\
uniform mat4 model;														    \n\
																			\n\
void main()																	\n\
{																			\n\
		gl_Position = model * vec4(pos, 1.0); 								\n\
		vColor = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);						\n\
}";

// Fragment Shader
static const char* fShader = "										\n\
#version 330														\n\
																	\n\
in vec4 vColor;														\n\
																	\n\
out vec4 color;														\n\
																	\n\
void main()															\n\
{																	\n\
	color = vColor;													\n\
}";

void CreateTriangle()
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

	glGenVertexArrays(1, &VAO);						// Generate vertex array object names (number of Vertex Array object names, a pointer to GLuint array where the names are stored)
	glBindVertexArray(VAO);							// Binds VAO to make it the current context. What is later called to initiate rendering

		glGenBuffers(1, &IBO);						// Generate index array buffer (number of Index Array objects, a pointer to GLuint Index Array where the names are stored)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);	// Bind IBO to make it the current context.
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // Creates and initializes IBO data store. similar to the buffer data below, but for indices instead of vertices

			glGenBuffers(1, &VBO);					// Generate buffer object names (number of buffer object names, a pointer to GLuint array where the names are stored)
			glBindBuffer(GL_ARRAY_BUFFER, VBO);		// Bind named buffer object. (Type of data in buffer, buffer object)
				glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Creates and initializes buffer object's "data store". Passed to the "in" variable

				glVertexAttribPointer(0,			// GLuint index. This is the location = 0 refered to in the vertex shader
					3,								// GLint size. Number of components. Must be 1-4
					GL_FLOAT,						// Glenum type. Type of data of each array componenent
					GL_FALSE,						// GLboolean normalized. Should the value be normalized or not.
					0,								// GLsizei stride. The byte offset between consecutive generic vertex attributes. 0 indicates tight packing
					0);								// const void * pointer. Offset of first component of first generic vertex attribute in the data store off the buffer currently bound
				glEnableVertexAttribArray(0);		// index to enable. So 0 indicates the array above

			glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
} 

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(theShader, sizeof(eLog), NULL, eLog);
		printf("Error compiling the %d program: '%s'\n", shaderType, eLog);
		return;
	}

	glAttachShader(theProgram, theShader);
}

void CompileShaders()
{
	shader = glCreateProgram();

	if (!shader)
	{
		printf("Error creating shader program!\n");
		return;
	}

	AddShader(shader, vShader, GL_VERTEX_SHADER);
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error validating  program: '%s'\n", eLog);
		return;
	}
	
	// assigns id to uniformModel. Makes uniformModel essentially an alias for "model"
	uniformModel = glGetUniformLocation(shader, "model"); 
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
	
	glEnable(GL_DEPTH_TEST);

	// create Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);

	CreateTriangle();
	CompileShaders();

	// Loop until window closed
	while (!glfwWindowShouldClose(mainWindow))
	{
		// Get + handle user input events
		glfwPollEvents();

		if (direction)
		{
			triOffset += triIncrement;
		}
		else
		{
			triOffset -= triIncrement;

		}
		if (abs(triOffset) >= triMaxOffset)
		{
			direction = !direction;
		}

		curAngle += 0.1f;
		if (curAngle >= 360)
		{
			curAngle -= 360;
		}

		if (sizeDirection)
		{
			curSize += 0.001f;
		}
		else
		{
			curSize -= 0.001f;
		}

		if (curSize >= maxSize || curSize <= minSize)
		{
			sizeDirection = !sizeDirection;
		}
		// Clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shader); // start using the shader program
		
			// Initialize transformation matrix from triOffset. The next three line are just GLM, they aren't openGL. 
			// The order of transformations is essentialy backwards
			glm::mat4 model(1.0f); // initialize simple 4x4 identity matrix using GLM
			//model = glm::translate(model, glm::vec3(triOffset, 0.0f, 0.0f)); // glm function builds matrix to translate in direction and magnitude of vector
			model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); // (matrix, angle of rotation, axis of rotation). This must concatenate the previous matrix
			model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f)); // (matrix, (scale coordinates))

			// pass the model matrix initialized above to shader program
			glUniformMatrix4fv(uniformModel, // location of matrix in shader
				1, // number of matrices to pass(?)
				GL_FALSE, // transpose matrix?
				glm::value_ptr(model)); // matrix to pass to shader matrix

			glBindVertexArray(VAO); // start operating on shader vertex array

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

					glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
			glBindVertexArray(0);

		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
	}

	return 0;
}