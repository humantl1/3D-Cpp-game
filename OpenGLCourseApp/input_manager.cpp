#include "SDL_ttf.h"

#include "input_manager.h"
#include "SDL_Window.h"

InputManager::InputManager() : exit_prompt{false} {}

void InputManager::ProcessInput() {
  SDL_Event event;

  SDL_PollEvent(&event);

  switch (event.type) {
    case SDL_QUIT: {
      exit_prompt = true;
      break;
    }
    case SDL_KEYDOWN: {
      if (event.key.keysym.sym == SDLK_ESCAPE ||
          event.key.keysym.sym == SDLK_q) {
        exit_prompt = true;
      }
      break;
    }
    default: {
      break;
    }
  }
}

InputManager::~InputManager() {}
