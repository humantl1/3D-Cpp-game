#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <imgui.h>
#include <lua.hpp>
#include <sol.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CommonValues.h"

#include "Window.h"
#include "SDL_Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
#include "Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;

// Initialize uniform variables
GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0,
		uniformDirectionalLightTransform = 0, uniformOmniLightPos = 0, uniformFarPlane = 0;

SDL_Window main_window;
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

Skybox skybox;

unsigned int pointLightCount = 0;
unsigned int spotLightCount = 0;
 
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

GLfloat asteroidAngle = 0.0f;

std::vector<Mesh*> meshList;

std::vector<Shader> shaderList; // potentially allows multiple rendering shaders
Shader directionalShadowShader;
Shader omniShadowShader;

static const char* vShader = "Shaders/shader.vert";
#include <sol.hpp>
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

// For demonstration of coding 3D image from scratch
// Manually define pyramid object and floor plane
void CreateObjects()
{
	// Pyramids:

	// Indices refer to each unique vertex of the pyramid
	// Since each vertex is used by 3 adjoining faces, this elimates redundacy
	unsigned int indices[] = 
	{
		0, 3, 1, // left triangle
		1, 3, 2, // right triangle
		2, 3, 0, // front triangle
		0, 1, 2  // bottom triangle
	};

	// Each unique vertex that composes a pyramid
	// Expanded to consist of positional, texture, and normal data
	// Although a 1D array, each line is referenced from the start of the line via the
	// Vertex Attribute Pointers in _Mesh.cpp
	GLfloat vertices[] = 
	{
		// x     y     z		 u     v		     normals
		-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,		// bottom left
		0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,		// "back"
		1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,		// bottom right
		0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f		// top 
	};

	// Floor plane:
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

// Initialize shaders from files (main shader, directional shadow shader, omni shadow shader)
void CreateShaders()
{
	// Initialize main shader and add to shaderList
	// Potentially allows for multiple draw shaders
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	// Shadow shaders
	directionalShadowShader = Shader();
	directionalShadowShader.CreateFromFiles("Shaders/directional_shadow_map.vert", "Shaders/directional_shadow_map.frag");
	omniShadowShader = Shader();
	omniShadowShader.CreateFromFiles("Shaders/omni_shadow_map.vert", "Shaders/omni_shadow_map.geom", "Shaders/omni_shadow_map.frag");
}


// Render passes

// Draw objects to screen
/* General Object Rendering Procedure:
*  1. Initialize 4x4 identity matrix using GLM 
*  2. Add rotation
*  3. Add translation
*  4. Add scaling
*  5. Pass GLM matrix to shader
*  6. Initialize texture to use
*  7. Initialize material to use
*  8. Render mesh or model
*/
void RenderScene()
{
		// triangle 1
		glm::mat4 model(1.0f); // initialize simple 4x4 identity matrix using GLM
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));

		// pass the model matrix initialized above to shader program
		glUniformMatrix4fv(uniformModel, // location of matrix in shader
			1,							 // number of matrices to pass(?)
			GL_FALSE,					 // transpose matrix?
			glm::value_ptr(model));		 // matrix to pass to shader matrix

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
}

/* Draw to directional shadow map
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
// 1.
	directionalShadowShader.UseShader();

// 2.
	// set viewport to same dimensions as framebuffer
	glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight()); 

// 3.
	light->GetShadowMap()->Write();		// bind FBO framebuffer
	glClear(GL_DEPTH_BUFFER_BIT);		// clear any previous depth buffer data

// 4.
	uniformModel = directionalShadowShader.GetModelLocation();				// assign uniformModel shader ID
	glm::mat4 lightTransform = light->CalculateLightTransform();
	directionalShadowShader.SetDirectionalLightTransform(&lightTransform);	// assign uniformDirectionalLightTransform ID

// 5.
	directionalShadowShader.Validate();
	RenderScene(); // Render scene from light's perspective

// 6.
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/* Draw to omni shadow map
*  Procedure:
*  1. Set directional shadow shader as active
*  2. Set viewport to framebuffer dimensions
*  3. Set FBO as framebuffer and clear old data
*  4. Assign the omni_shadow_map.vert and .frag uniform IDs
*  5. Pass light position and far plane light reaches
*  6. Assign uniforms for each face of cube map used for omni-directional shadows
*  7. Render the scene (from the perspective of a light)
*  8. Rebind the default framebuffer for writing to the screen
*/
void OmniShadowMapPass(PointLight* light)
{
// 1.
	omniShadowShader.UseShader();

#include <sol.hpp>
// 2.
	// set viewport to same dimensions as framebuffer
	glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight()); 

// 3.
	light->GetShadowMap()->Write();		// bind FBO framebuffer
	glClear(GL_DEPTH_BUFFER_BIT);		// clear any previous depth buffer data

// 4.
	// set shader uniform IDs
	uniformModel = omniShadowShader.GetModelLocation();					// in omni_shadow_map.vert
	uniformOmniLightPos = omniShadowShader.GetOmniLightPosLocation();	// in omni_shadow_map.frag 
	uniformFarPlane = omniShadowShader.GetFarPlaneLocation();			// in omni_shadow_map.frag 

// 5.
	// Pass light position and far plane distance into omni_shadow_map.frag
	glUniform3f(uniformOmniLightPos, light->GetPosition().x, light->GetPosition().y, light->GetPosition().z);
	glUniform1f(uniformFarPlane, light->GetFarPlane());

// 6.
	// Set light matrices for omni_shadow_map.geom
	omniShadowShader.SetLightMatrices(light->CalculateLightTransform());

// 7.
	omniShadowShader.Validate();
	RenderScene(); // Render scene from light's perspective

// 8.
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // rebind default frame buffer for drawing to screen
}

/* Draw to screen frame buffer
*  Procedure:
*  1. Define viewport dimensions
*  2. Clear window
*  3. Draw skybox
*  4. Select active shader
*  5. Assign uniforms
*  6. Initialize universal uniforms (projection, camera, and eye position) 
*  7. Set Lighting
*  8. Set Shadows
*  9. Set Flashlight
* 10. Draw Scene
*/
void RenderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{	
// 1.
	glViewport(0, 0, 1366, 768); // TODO: viewport function

// 2.
	// Clear window
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the color and depth buffers

// 3.
	// Draw skybox
	skybox.DrawSkybox(viewMatrix, projectionMatrix);

// 4. 
	// Indicate which shader to use
	shaderList[0].UseShader();	
	
// 5.
	// assign uniform handles
	uniformModel = shaderList[0].GetModelLocation();
	uniformProjection = shaderList[0].GetProjectionLocation();
	uniformView = shaderList[0].GetViewLocation();
	uniformEyePosition = shaderList[0].GetEyePositionLocation();
	uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
	uniformShininess = shaderList[0].GetShininessLocation();

// 6.
	// Set "universal" uniforms (projection, view, and eye position apply to entire scene)
	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix)); // pass projection matrix to shader program
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix)); // pass camera matrix to shader program
	glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

// 7. 
	// Set active lighting
	shaderList[0].SetDirectionalLight(mainLight);
	// shift following light textures up one (to 3) to avoid any textures being set to 0, the "spare" texture
	shaderList[0].SetPointLights(pointLights, pointLightCount, 3, 0);  // SetPointLights needs to occur before SetSpotLights due to light calc functions in shader.frag 
	shaderList[0].SetSpotLights(spotLights, spotLightCount, 3 + pointLightCount, pointLightCount); // spotlights occupy consecutive indices after pointLights
	glm::mat4 lightTransform = mainLight.CalculateLightTransform();
	shaderList[0].SetDirectionalLightTransform(&lightTransform); // used in shader.vert to calculate fragment positions in relation to the directional light

// 8. 
	// standard textures are unit 0, so set shadow map texture to unit 1
	mainLight.GetShadowMap()->Read(GL_TEXTURE2); // set to 2 to avoid 0... see below
	shaderList[0].SetTexture(1);                 // set to 1 to avoid setting to 0 (0 will be unused "spare" texture)
	shaderList[0].SetDirectionalShadowMap(2);    // see above

// 9.
	// Set "flashlight" slightly below camera to have more flashlight-like behavior 
	glm::vec3 lowerLight = camera.getCameraPosition();
	lowerLight.y -= 0.5f;
	spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

// 10.
	// Draw scene
    shaderList[0].Validate();
	RenderScene();
}

// End render passes

int main(int argc, char* argv[]) // SDL requires command line inputs be accomodated
{
	sol::state lua;
	lua.open_libraries(sol::lib::base);
  main_window = SDL_Window(1366, 768);
	main_window.Initialize();
	//mainWindow = Window(1366, 768);
	//mainWindow.initialize();
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
								0.0f, 0.5f,				// ambient & diffuse intensities 
								5.0f, -15.0f, -10.0f);	// direction light is shining

	pointLights[0] = PointLight(1024, 1024,				// resolution of shadow map
								0.01f, 100.0f,			// near and far plane distance
								0.0f, 0.0f, 1.0f,		// rgb color values
								0.0f, 0.4f,				// ambient and directional light intensity
								2.0f, 2.0f, 0.0f,		// position of light source
								0.3f, 0.1f, 0.1f);		// attenuation factors (strength of light over distance)
	pointLightCount++;

	pointLights[1] = PointLight(1024, 1024,				// resolution of shadow map
								0.01f, 100.0f,			// near and far plane distance
								0.0f, 1.0f, 0.0f,		// rgb color values
								0.0f, 0.1f,				// ambient and directional light intensity
								-4.0f, 2.0f, 0.0f,		// position of light source
								0.3f, 0.2f, 0.1f);		// attenuation factors (strength of light over distance)
	pointLightCount++;

	spotLights[0] = SpotLight(1024, 1024,				// resolution of shadow map
								0.01f, 100.0f,			// near and far plane distance
								1.0f, 1.0f, 1.0f,		// rgb color values
								0.0f, 2.0f,				// ambient and directional light intensity
								0.0f, 0.0f, 0.0f,		// position of light source
								0.0f, -1.0f, 0.0f,		// direction of light rays
								1.0f, 0.0f, 0.0f,		// attenuation factors (strength of light over distance				
								20.0f);					// angle of light focus
	spotLightCount++;

	spotLights[1] = SpotLight(1024, 1024,				// resolution of shadow map
								0.01f, 100.0f,			// near and far plane distance
								1.0f, 1.0f, 1.0f,		// rgb color values
								0.0f, 1.0f,				// ambient and directional light intensity
								0.0f, -1.5f, 0.0f,		// position of light source
								-100.0f, -1.0f, 0.0f,	// direction of light rays
								1.0f, 0.0f, 0.0f,		// attenuation factors (strength of light over distance				
								20.0f);					// angle of light focus
	spotLightCount++;

	// Initialize array of skybox faces in explicit order: +x, -x, +y, -y, +z, -z
	std::vector<std::string> skyboxFaces;	
	skyboxFaces.push_back("Textures/Skybox/moonwaw_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/moonwaw_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/moonwaw_up.tga");
	skyboxFaces.push_back("Textures/Skybox/moonwaw_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/moonwaw_ft.tga");
	skyboxFaces.push_back("Textures/Skybox/moonwaw_bk.tga");


	skybox = Skybox(skyboxFaces);

	// initial camera values
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f),	// Camera start position
	  glm::vec3(0.0f, 1.0f, 0.0f),				// Camera start up direction
		-90.0f,										// Camera start yaw
		0.0f,										// Camera start pitch
		5.00f,										// Camera move speed
		0.2f);										// Camera turn speed

	// projection matrix doesn't change
	glm::mat4 projection = glm::perspective(
		glm::radians(60.0f), // fov from top to bottom
		(GLfloat)main_window.GetBufferWidth() / (GLfloat)main_window.GetBufferHeight(), // aspect ratio
		0.1f, // near z
		100.0f); // far z

	// Main loop
	while (main_window.GetIsRunning())
	{
		main_window.ProcessInput();
		// Delta Time
		GLfloat now = glfwGetTime(); // returns time in seconds
	  deltaTime = now - lastTime;
		lastTime = now;

	//	// Get + handle user input events
	//	glfwPollEvents();
	//	camera.keyControl(mainWindow.getKeys(), deltaTime);
	//	camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

	//	if (mainWindow.getKeys()[GLFW_KEY_L])
	//	{
	//		spotLights[0].Toggle();
	//		mainWindow.getKeys()[GLFW_KEY_L] = false;
	//	}

		// Get shadows
		DirectionalShadowMapPass(&mainLight); // render scene to shadow map frame buffer
		for (size_t i = 0; i < pointLightCount; i++)
		{
			OmniShadowMapPass(&pointLights[i]);
		}
		for (size_t i = 0; i < spotLightCount; i++)
		{
			OmniShadowMapPass(&spotLights[i]);
		}

		// Draw to screen
		RenderPass(projection, camera.calculateViewMatrix());

		glUseProgram(0);

		main_window.SwapBuffer();
	}

	return 0;
}