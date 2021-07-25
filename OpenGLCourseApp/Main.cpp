#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL/glew.h> //TODO: neccesary?
#include <GLFW/glfw3.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <imgui.h>
#include <lua.hpp>
#include <sol.hpp>

#include <glm/glm.hpp> //TODO: neccesary?
#include <glm/gtc/matrix_transform.hpp> //TODO: neccesary?
#include <glm/gtc/type_ptr.hpp> //TODO: neccesary?

#include "CommonValues.h"

#include "SDL_Window.h"
#include "render_manager.h"
#include "input_manager.h"
#include "Update.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h" //TODO: neccesary?
#include "DirectionalLight.h" //TODO: neccesary?
#include "PointLight.h" //TODO: neccesary?
#include "SpotLight.h" //TODO: neccesary?
#include "Material.h" //TODO: neccesary?
#include "Model.h" //TODO: neccesary?
#include "Skybox.h" //TODO: neccesary?

SDL_Window main_window;
RenderManager renderer;
InputManager input;
Update update;
Camera camera;

// SDL requires command line inputs be accomodated
int main(int argc, char* argv[]) {
  sol::state lua;
  lua.open_libraries(sol::lib::base);
  main_window = SDL_Window(1366, 768);
  main_window.Initialize();
  renderer.Initialize("Shaders/shader.vert", "Shaders/shader.frag",
                      main_window.GetBufferWidth(),
                      main_window.GetBufferHeight());
  renderer.CreateObjects();
  renderer.CreateShaders();

  // initial camera values
  camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f),  // Camera start position
                  glm::vec3(0.0f, 1.0f, 0.0f),  // Camera start up direction
                  -90.0f,                       // Camera start yaw
                  0.0f,                         // Camera start pitch
                  5.00f,                        // Camera move speed
                  0.2f);                        // Camera turn speed

  // Main loop
  while (!input.GetExitPrompt()) {
    input.ProcessInput();
    update.RunGameLoop();

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
    // render scene to shadow map frame buffer
    renderer.DirectionalShadowMapPass(update.GetDeltaTime());
    for (size_t i = 0; i < renderer.pointLightCount; i++) {
      renderer.OmniShadowMapPass(&renderer.pointLights[i], update.GetDeltaTime());
    }
    for (size_t i = 0; i < renderer.spotLightCount; i++) {
      renderer.OmniShadowMapPass(&renderer.spotLights[i], update.GetDeltaTime());
    }

    // Draw to screen
    renderer.RenderPass(camera.calculateViewMatrix(), camera,
               update.GetDeltaTime());

    glUseProgram(0);
    main_window.SwapBuffer();
  }
  
  main_window.Destroy();
  return 0;
}