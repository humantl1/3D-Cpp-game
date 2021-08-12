#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <imgui.h>
#include <lua.hpp>
#include <sol.hpp>

#include "CommonValues.h"

#include "game.h"
#include "SDL_Window.h"

// SDL requires command line inputs be accomodated
int main(int argc, char* argv[]) {
  sol::state lua;
  lua.open_libraries(sol::lib::base);
  Game* game = new Game();
  game->Run();
  
  delete game;
  return 0;
}