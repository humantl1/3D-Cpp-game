#include "SDL_ttf.h"

#include "input_manager.h"
#include "SDL_Window.h"

InputManager::InputManager() : exit_prompt_{false} {}

void InputManager::ProcessInput() {
  SDL_PollEvent(&event_);

  switch (event_.type) {
    // Window Event
    case SDL_QUIT: 
      exit_prompt_ = true;
      break;
    
    // KeyPress Event
    case SDL_KEYDOWN: 
      switch (event_.key.keysym.sym) {
      case SDLK_ESCAPE:
        exit_prompt_ = true;
        break;
      case SDLK_q:
        exit_prompt_ = true;
        break;
      default:
        break;
      }
      break;

    default: 
      break;
  }
}

InputManager::~InputManager() {}
