#include <iostream>

#include "../lib/glm/glm.hpp"

#include "game.h"
#include "constants.h"
#include "components/transform_component.h"
#include "components/sprite_component.h"
#include "components/keyboard_component.h"

EntityManager manager_;
AssetManager* Game::asset_manager_ = new AssetManager(&manager_);
SDL_Renderer* Game::renderer_;
SDL_Event Game::event_;

Game::Game() : ticksLastFrame_{0} {
  this->is_running_ = false;
}

Game::~Game() {}

bool Game::IsRunning() const {
  return this->is_running_;
}

// initialize window, renderer
void Game::Initialize(int width, int height) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    std::cerr << "Error initializing SDL." <<std::endl;
    return;
  }

  window_ = SDL_CreateWindow( // Create window via SDL API
    NULL,                    // window title
    SDL_WINDOWPOS_CENTERED,  // xpos of window
    SDL_WINDOWPOS_CENTERED,  // ypos of window
    width,                   // width of window
    height,                  // height of window
    SDL_WINDOW_BORDERLESS);  // flags (no border)

  if (!window_) {
    std::cerr << "Error creating SDL window." << std::endl;
    return;
  }

  renderer_ = SDL_CreateRenderer(  // Create 2D renderer context for window
    window_,                       // window where rendering is displayed
    -1,                           // index, -1 to initialize first supported rendering driver
    0);                           // optional flags

  if (!renderer_) {
    std::cerr << "Error creating SDL renderer." << std::endl;
  }

  LoadScene(0);

  is_running_ = true;
}

void Game::LoadScene(int scene_number) {
  // Include new assets to asset manager map
  asset_manager_->AddTexture("tank-image",
    "./assets/images/tank-big-right.png");
  asset_manager_->AddTexture("chopper-image",
    "./assets/images/chopper-spritesheet.png");
  asset_manager_->AddTexture("rock-image",
    "./assets/images/rock-big-3.png");

  // Include entities and their components
  Entity& entity_tank(manager_.AddEntity("tank"));
  entity_tank.AddComponent<TransformComponent>(0, 0, 20, 20, 32, 32, 1);
  entity_tank.AddComponent<SpriteComponent>("tank-image");

  Entity& entity_chopper(manager_.AddEntity("chopper"));
  entity_chopper.AddComponent<TransformComponent>(
    300, 300, 0, 0, 32, 32, 1);
  entity_chopper.AddComponent<SpriteComponent>("chopper-image",
    2, 90, true, false);
  entity_chopper.AddComponent<KeyboardComponent>("up", "right", "down", "left",
  "space");

  Entity& entity_rock(manager_.AddEntity("rock"));
  entity_rock.AddComponent<TransformComponent>(250, 125, 0, 0, 32, 32, 1);
  entity_rock.AddComponent<SpriteComponent>("rock-image");

  manager_.ListEntities();
  std::cout << std::boolalpha << "Rock has transform component? " <<
    entity_rock.HasComponent<TransformComponent>() <<
    std::noboolalpha << std::endl;
}

// Handle input
void Game::ProcessInput() {
  // Queue events and pass to event object
  SDL_PollEvent(&event_); // Returns 1 if event pending. If there is a "pending event" it is stored in SDL_Event.

  // Handle event types
  switch (event_.type) {
    // Exit program if window 'x' button is pressed
    case SDL_QUIT: {
      is_running_ = false;
      break;
    }

    // Exit program if 'Escape' is pressed
    case SDL_KEYDOWN: {
      if (event_.key.keysym.sym == SDLK_ESCAPE) {
        is_running_ = false;
      }
    }
    default: {
      break;
    }
  }
}

void Game::Update() {
  // wait until 16.6 ms has ellapsed since the last frame
  float timeToWait = kFrameTargetTime - (SDL_GetTicks() - ticksLastFrame_);

  // delay if rendering completed before desired fps
  if (timeToWait > 0 && timeToWait <= kFrameTargetTime) {
    SDL_Delay(timeToWait);
  }

  // delta time is difference in ticks from last frame converted to seconds
  float delta_time = (SDL_GetTicks() - ticksLastFrame_) / 1000.0f;

  // Clamp deltaTime to max value
  delta_time = (delta_time > kMaxDeltaTime) ? kMaxDeltaTime : delta_time;

  // set new ticks for the current frame to be used in the next pass
  ticksLastFrame_ = SDL_GetTicks();

  manager_.Update(delta_time);
}

void Game::Render() {
  SDL_SetRenderDrawColor(renderer_, 21, 21, 21, 255);
  SDL_RenderClear(renderer_); // clear back buffer

  if (manager_.HasNoEntities()) {
    return;
  }

  manager_.Render();

  // swap buffer
  SDL_RenderPresent(renderer_);
}

void Game::Destroy() {
  SDL_DestroyRenderer(renderer_);
  SDL_DestroyWindow(window_);
  SDL_Quit();
}

