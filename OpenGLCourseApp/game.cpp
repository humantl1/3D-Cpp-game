#include "render_manager.h"

#include "game.h"
#include "transform_component.h"

Game::Game() : buffer_height_{kBufferStartHeight},
               buffer_width_{kBufferStartWidth},
               window_{nullptr},
               entity_manager_{nullptr},
               asset_manager_ {nullptr},
               camera_ {nullptr} {}

void Game::Initialize() { 
  window_ = new SDL_Window(kBufferStartWidth, kBufferStartHeight);
  window_->Initialize();
  asset_manager_ = new AssetManager();
  entity_manager_ = new EntityManager();
  renderer_.Initialize(buffer_width_, buffer_height_);
  renderer_.CreateObjects();
  renderer_.CreateShaders();
  // initial camera values
  //camera_ = Camera(glm::vec3(0.0f, 1.0f, 0.0f),  // Camera start position
  //                 glm::vec3(0.0f, 10.0f, 0.0f));  // Camera start up direction
  LoadScene(0);
}

void Game::LoadScene(int scene_number) {
  camera_ = &entity_manager_->AddEntity("camera");
  camera_->AddComponent<TransformComponent>(glm::vec3(0.0f, 0.0f, 0.0f),
                                          glm::vec3(0.0f, 0.0f, 0.0f),
                                          -90.0f,
                                          0.0f,
                                          0.0f,
                                          1.0f,
                                          5.0f,
                                          0.2f);
  camera_->AddComponent<Camera>(glm::vec3(0.0f, 1.0f, 0.0f),  // Camera start position
                                     glm::vec3(0.0f, 10.0f, 0.0f));  // Camera start up direction);
}

void Game::Run() {
  this->Initialize();

  // Main loop
  while (!input_.GetExitPrompt()) {
    input_.ProcessInput();
    update_.RunGameLoop(*entity_manager_);

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
    renderer_.RenderPass(camera_->GetComponent<Camera>()->calculateViewMatrix(),
                         *camera_->GetComponent<Camera>(),
                         update_.GetDeltaTime());

    
    glUseProgram(0);
    window_->SwapBuffer();
  }
}

Game::~Game() {
  window_->Destroy();
  delete entity_manager_;
  delete asset_manager_;
}
