#include <SDL2/SDL_render.h>
#include <iostream>

#include "game.h"
#include "constants.h"
#include "../lib/glm/glm.hpp"

EntityManager manager;
SDL_Renderer* Game::renderer_;


Game::Game() : ticksLastFrame_{0} {
  this->is_running_ = false;
}

Game::~Game() {}

bool Game::IsRunning() const {
  return this->is_running_;
}

// TODO: temp vars for testing
float projectile_pos_x = 0.0f;
float projectile_pos_y = 0.0f;
float projectile_vel_x = 20;
float projectile_vel_y = 20;

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

  is_running_ = true;
}

// Handle input
void Game::ProcessInput() {
  // Create object to store events
  SDL_Event event;       // A structure containg all SDL event structures
  
  // Queue events and pass to event object
  SDL_PollEvent(&event); // Returns 1 if event pending. If there is a "pending event" it is stored in SDL_Event. 

  // Handle event types
  switch (event.type) {
    // Exit program if window 'x' button is pressed
    case SDL_QUIT: {
      is_running_ = false;
      break;
    }

    // Exit program if 'Escape' is pressed
    case SDL_KEYDOWN: {
      if (event.key.keysym.sym == SDLK_ESCAPE) {
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
  float deltaTime = (SDL_GetTicks() - ticksLastFrame_ / 1000.0f);

  // Clamp deltaTime to max value
  deltaTime = (deltaTime > kMaxDeltaTime) ? kMaxDeltaTime : deltaTime;

  // set new ticks for the current frame to be used in the next pass
  ticksLastFrame_ = SDL_GetTicks();

  projectile_pos_x += projectile_vel_x * deltaTime;
  projectile_pos_y += projectile_vel_y * deltaTime;
}

void Game::Render() {
  SDL_SetRenderDrawColor(renderer_, 21, 21, 21, 255);
  SDL_RenderClear(renderer_); // clear back buffer

  SDL_Rect projectile {
    static_cast<int>(projectile_pos_x),
    static_cast<int>(projectile_pos_y),
    10,
    10};

  SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255);
  SDL_RenderFillRect(renderer_, &projectile);

  // swap buffer
  SDL_RenderPresent(renderer_);
}

void Game::Destroy() {
  SDL_DestroyRenderer(renderer_);
  SDL_DestroyWindow(window_);
  SDL_Quit();
}

