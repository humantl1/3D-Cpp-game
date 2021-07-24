/*
 * load texture from disc
 * draw texture to screen
 */

#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <SDL2/SDL.h>

#include "game.h"

class TextureManager {
 public:
  static SDL_Texture* LoadTexture(const char* file_path);
  static void Draw(SDL_Texture* texture, SDL_Rect source_rectangle,
      SDL_Rect destination_rectangle, SDL_RendererFlip flip);
 private:
};

#endif
