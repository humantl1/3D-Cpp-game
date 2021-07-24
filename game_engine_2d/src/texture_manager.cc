#include "texture_manager.h"

SDL_Texture* TextureManager::LoadTexture(const char* file_path) {
  SDL_Surface* surface = IMG_Load(file_path);
  SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer_, surface);
}

void TextureManager::Draw(SDL_Texture* texture, SDL_Rect source_rectangle,
      SDL_Rect destination_rectangle, SDL_RendererFlip flip) {
  SDL_RenderCopyEx(
      Game::renderer_,
      texture,
      &source_rectangle,
      &destination_rectangle,
      0.0,
      NULL,
      flip);
}
