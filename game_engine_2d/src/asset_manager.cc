#include "asset_manager.h"

AssetManager::AssetManager(EntityManager* manager) : manager_ {manager} {}

void AssetManager::ClearData() {
  textures_.clear();
}

void AssetManager::AddTexture(std::string texture_id, const char* file_path) {
  textures_.emplace(texture_id, TextureManager::LoadTexture(file_path));
}

SDL_Texture* AssetManager::GetTexture(std::string texture_id) {
  return textures_[texture_id];
}
