#pragma once

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "SDL_Window.h"
#include "render_manager.h"
#include "input_manager.h"
#include "Camera.h"
#include "Update.h"
#include "entity_manager.h"
#include "asset_manager.h"

class Game {
 public:
  GLint buffer_height_, buffer_width_;
  RenderManager renderer_;
  InputManager input_;
  Update update_;
  Entity* camera_;
  SDL_Window* window_;
  EntityManager* entity_manager_;
  AssetManager* asset_manager_;

  Game();
  void Initialize();
  void Run();
  void LoadScene(int scene_number);
  ~Game();
 private:
};
