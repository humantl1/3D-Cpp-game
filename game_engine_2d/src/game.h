#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "entity.h"
#include "component.h"
#include "entity_manager.h"

class Game {
 private:
  bool is_running_;
  SDL_Window* window_;
 public:
  int ticksLastFrame_;
  static SDL_Renderer *renderer_;

  Game();
  ~Game();
  bool IsRunning() const;
  void Initialize(int width, int height);
  void LoadScene(int scene_number);
  void ProcessInput();
  void Update();
  void Render();
  void Destroy();
};

#endif
