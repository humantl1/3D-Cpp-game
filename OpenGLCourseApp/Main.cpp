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

#include "CommonValues.h"

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
#include "Model.h"

const float toRadians = 3.14159265f / 180.0f;

// Initialize uniform variables
GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;

Window mainWindow;
Camera camera;

Texture rockTexture;
Texture asteroidTexture;

Material shinyMaterial;
Material dullMaterial;

Model xwing;
Model asteroid;
Model asteroid2;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

unsigned int pointLightCount = 0;
unsigned int spotLightCount = 0;
 
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

GLfloat asteroidAngle = 0.0f;

std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Shader directionalShadowShader;

static const char* vShader = "Shaders/shader.vert";
static const char* fShader = "Shaders/shader.frag";


// Phong shading. Average normals of each face of shared vertices
void calcAverageNormals(unsigned int* indices, // array of indices	
	unsigned int indiceCount,	// size of indices array
    GLfloat* vertices,		// array of vertices
	unsigned int verticeCount,	// size of vertices array
	unsigned int vLength,		// number of columns in vertices array
	unsigned int normalOffset)	// offset from start of vertices array row to first normal index for that row
{
	// iterates through each face, adding each face's normalized normal to vertices' normals
	for (size_t i = 0; i < indiceCount; i += 3) // i is equivalent to the current face (aka triangle)
	{
		// assigns starting index of vertices array row corresponding to the vertex of the current triangle
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;

		// calculate two vectors from vertices of the current face, calculate surface normal from cross product of vectors, normalize surface normal
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		// in_ are now aliases for starting index of normal vals for the current face
		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		// add calculated normalized surface normal to corresponding vertex normal in vertices array
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	// normalize (average) vertices normals
	for (size_t i = 0; i < verticeCount / vLength; i++) // for each row of vertices[]
	{
		unsigned int nOffset = i * vLength + normalOffset;										 // set index for start of normal indices for the ith row
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);			 // initialize new vector from normal values from ith row
		vec = glm::normalize(vec);																 // normalize this new vector
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z; // reinput normalized values into vertices[] normal values
	}
}

void CreateObjects()
{
	// Pyramids:
	unsigned int indices[] = 
	{
		0, 3, 1, // left triangle
		1, 3, 2, // right triangle
		2, 3, 0, // front triangle
		0, 1, 2  // bottom triangle
	};

	GLfloat vertices[] = 
	{
		// x     y     z		 u     v		     normals
		-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,		// bottom left
		0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,		// "back"
		1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,		// bottom right
		0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f		// top 
	};

	// Floor:
	unsigned int floorIndices[] =
	{
		0, 2, 1, // back left triangle
		1, 2, 3  // front right triangle
	};

	GLfloat floorVertices[] =
	{
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,	// back left
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,	// back right
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,	// front left
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f	// front right
	};

	// calculate averaged normals for pyramids
	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	// pyramid 1
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	// pyramid 2
	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	// floor
	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);
} 

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	directionalShadowShader = Shader();
	directionalShadowShader.CreateFromFiles("Shaders/directional_shadow_map.vert", "Shaders/directional_shadow_map.frag");
}


// Render passes

void RenderScene()
{

		// Render Objects:

		// triangle 1
		// Initialize transformation matrix from triOffset.
		glm::mat4 model(1.0f); // initialize simple 4x4 identity matrix using GLM
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f)); // glm function builds matrix to translate in direction and magnitude of vector

		// Examples of rotate and scale matrices
		//model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); // (matrix, angle of rotation, axis of rotation). This must concatenate the previous matrix
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f)); // (matrix, (scale coordinates))

		// pass the model matrix initialized above to shader program
		glUniformMatrix4fv(uniformModel, // location of matrix in shader
			1, // number of matrices to pass(?)
			GL_FALSE, // transpose matrix?
			glm::value_ptr(model)); // matrix to pass to shader matrix

		rockTexture.UseTexture();
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[0]->RenderMesh();
		

		// triangle 2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 3.0f, -2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		asteroidTexture.UseTexture();
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();


		// floor
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, -6.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		asteroidTexture.UseTexture();
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		// xwing
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-7.0f, 0.0f, 10.0f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		xwing.RenderModel();

		asteroidAngle += 0.2f;
		if (asteroidAngle > 360.0f)
		{
			asteroidAngle = 0.1f;
		}

		// asteroid
		model = glm::mat4(1.0f);
		model = glm::rotate(model, asteroidAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(4.0f, 6.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.003f, 0.003f, 0.003f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		asteroid.RenderModel();

		// asteroid2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-2.0f, 4.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		asteroid2.RenderModel();

		// End Object Rendering.
}

/* Draw to shadow map
*  Procedure:
*  1. Set directional shadow shader as active
*  2. Set viewport to framebuffer dimensions (seems funny this isn't the other way around, huh?)
*  3. Set FBO as framebuffer and clear old data
*  4. Assign the directional_shadow_map.vert uniform IDs: model and directionalLightTranform
*  5. Render the scene (from the perspective of a light)
*  6. Rebind the default framebuffer for writing to the screen
*/
void DirectionalShadowMapPass(DirectionalLight* light)
{
	directionalShadowShader.UseShader();

	// set viewport to same dimensions as framebuffer
	glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight()); 

	light->GetShadowMap()->Write();		// bind FBO framebuffer
	glClear(GL_DEPTH_BUFFER_BIT);		// clear any previous depth buffer data

	uniformModel = directionalShadowShader.GetModelLocation();									// assign uniformModel shader ID
	glm::mat4 lightTransform = light->CalculateLightTransform();
	directionalShadowShader.SetDirectionalLightTransform(&lightTransform);	// assign uniformDirectionalLightTransform ID

	RenderScene(); // Render scene from light's perspective

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/* Draw to screen frame buffer
*  Procedure:
*  1. 
*/
void RenderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{
	shaderList[0].UseShader();	
	
	// assign uniform handles
	uniformModel = shaderList[0].GetModelLocation();
	uniformProjection = shaderList[0].GetProjectionLocation();
	uniformView = shaderList[0].GetViewLocation();
	uniformEyePosition = shaderList[0].GetEyePositionLocation();
	uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
	uniformShininess = shaderList[0].GetShininessLocation();

	glViewport(0, 0, 1366, 768); // TODO: viewport function

	// Clear window
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the color and depth buffers


	// Set "universal" uniforms (projection, view, and eye position apply to entire scene)
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix)); // pass projection matrix to shader program
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix)); // pass camera matrix to shader program
	glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);


	// Set active lighting
	shaderList[0].SetDirectionalLight(mainLight);
	shaderList[0].SetPointLights(pointLights, pointLightCount);
	shaderList[0].SetSpotLights(spotLights, spotLightCount);
	glm::mat4 lightTransform = mainLight.CalculateLightTransform();
	shaderList[0].SetDirectionalLightTransform(&lightTransform); // used in shader.vert to calculate fragment positions in relation to the directional light

	// standard textures are unit 0, so set shadow map texture to unit 1
	mainLight.GetShadowMap()->Read(GL_TEXTURE1); 
	shaderList[0].SetTexture(0);
	shaderList[0].SetDirectionalShadowMap(1);

	// Set "flashlight" slightly below camera to have more flashlight-like behavior 
	glm::vec3 lowerLight = camera.getCameraPosition();
	lowerLight.y -= 0.5f;
	spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

	RenderScene();
}

// End render passes

int main()
{
	mainWindow = Window(1366, 768);
	mainWindow.initialize();
	CreateObjects();
	CreateShaders();


	// Set Textures
 	rockTexture = Texture((char*)"Textures/rock.png");
	rockTexture.LoadTexture();
	asteroidTexture = Texture((char*)"Textures/asteroid.png");
	asteroidTexture.LoadTexture();
 


	// Set Materials
	shinyMaterial = Material(1.0f, 256);
	dullMaterial = Material(0.3f, 4);

	// Load Models
	xwing = Model();
	xwing.LoadModel("Models/x-wing.obj");

	asteroid = Model();
	asteroid.LoadModel("Models/10464_Asteroid_v1_Iterations-2.obj");

	asteroid2 = Model();
	asteroid2.LoadModel("Models/Asteroid_1_LOW_MODEL_.obj");

	// TODO: handle lights in classes
	// Set Lighting
	mainLight = DirectionalLight(4096, 4096,			// resolution of shadow map
								1.0f, 1.0f, 1.0f,		// colors
								0.1f, 0.6f,				// ambient & diffuse intensities 
								0.0f, -15.0f, -10.0f);	// direction light is shining

	pointLights[0] = PointLight(0.0f, 0.0f, 1.0f,
								0.0f, 0.1f,
								0.0f, 0.0f, 0.0f,
								0.3f, 0.2f, 0.1f);
	//pointLightCount++;

	pointLights[1] = PointLight(0.0f, 1.0f, 0.0f,
								0.0f, 0.1f,
								-4.0f, 2.0f, 0.0f,
								0.3f, 0.1f, 0.1f);
	//pointLightCount++;

	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
								0.0f, 2.0f,
								0.0f, 0.0f, 0.0f,
								0.0f, -1.0f, 0.0f,
								1.0f, 0.0f, 0.0f,						
								20.0f);
		//spotLightCount++;

	spotLights[1] = SpotLight(1.0f, 1.0f, 1.0f,
								0.0f, 1.0f,
								0.0f, -1.5f, 0.0f,
								-100.0f, -1.0f, 0.0f,
								1.0f, 0.0f, 0.0f,						
								20.0f);
		//spotLightCount++;


	// initial camera values
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f),	// Camera start position
		glm::vec3(0.0f, 1.0f, 0.0f),				// Camera start up direction
		-90.0f,										// Camera start yaw
		0.0f,										// Camera start pitch
		5.00f,										// Camera move speed
		0.2f);										// Camera turn speed

	// projection matrix doesn't change
	glm::mat4 projection = glm::perspective(
		glm::radians(45.0f), // fov from top to bottom
		mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight(), // aspect ratio
		0.1f, // near z
		100.0f); // far z

	// Main loop
	while (!mainWindow.getShouldClose())
	{
		// Delta Time
		GLfloat now = glfwGetTime(); // returns time in seconds
		deltaTime = now - lastTime;
		lastTime = now;

		// Get + handle user input events
		glfwPollEvents();
		camera.keyControl(mainWindow.getKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		DirectionalShadowMapPass(&mainLight); // render scene to shadow map frame buffer
		RenderPass(projection, camera.calculateViewMatrix());

		glUseProgram(0);

		mainWindow.swapBuffer();
	}

	return 0;
}