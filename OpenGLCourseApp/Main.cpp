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

SDL_Window main_window;

// SDL requires command line inputs be accomodated
int main(int argc, char* argv[]) {
  sol::state lua;
  lua.open_libraries(sol::lib::base);
  main_window = SDL_Window(kBufferStartWidth, kBufferStartHeight);
  main_window.Initialize();
  Game* game = new Game(main_window, main_window.GetBufferHeight(),
                        main_window.GetBufferWidth());
  game->Run();
  
  delete game;
  main_window.Destroy();
  return 0;
}