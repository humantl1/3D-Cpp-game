#include "render_manager.h"

#include "game.h"

Game::Game(SDL_Window& window, GLint buffer_height, GLint buffer_width) 
  : buffer_height_{buffer_height}, buffer_width_{buffer_width},
      window_{window} {
  
}

void Game::Run() {
  renderer_.Initialize(buffer_width_, buffer_height_);
  renderer_.CreateObjects();
  renderer_.CreateShaders();

  // initial camera values
  camera_ = Camera(glm::vec3(0.0f, 0.0f, 0.0f),  // Camera start position
                   glm::vec3(0.0f, 1.0f, 0.0f),  // Camera start up direction
                   -90.0f,                       // Camera start yaw
                   0.0f,                         // Camera start pitch
                   5.00f,                        // Camera move speed
                   0.2f);                        // Camera turn speed

  // Main loop
  while (!input_.GetExitPrompt()) {
    input_.ProcessInput();
    update_.RunGameLoop();

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
    renderer_.DirectionalShadowMapPass(update_.GetDeltaTime());
    for (size_t i = 0; i < renderer_.pointLightCount; i++) {
      renderer_.OmniShadowMapPass(&renderer_.pointLights[i],
                                  update_.GetDeltaTime());
    }
    for (size_t i = 0; i < renderer_.spotLightCount; i++) {
      renderer_.OmniShadowMapPass(&renderer_.spotLights[i],
                                  update_.GetDeltaTime());
    }

    // Draw to screen
    renderer_.RenderPass(camera_.calculateViewMatrix(), camera_,
                         update_.GetDeltaTime());

    glUseProgram(0);
    window_.SwapBuffer();
  }
}
