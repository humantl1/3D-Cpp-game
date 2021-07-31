#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#if VSCODEWIN
#include "../../ExternalLibs/SDL2/include/SDL.h"
#elif VSCODELINUX
#include <SDL2/SDL.h>
#endif

#include "../texture_manager.h"
#include "../asset_manager.h"

class SpriteComponent : public Component {
 public:
  SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;
 private:
  SDL_Texture* texture;
  SDL_Rect sourceRectangle;
  SDL_Rect destinationRectangle;
};

#endif