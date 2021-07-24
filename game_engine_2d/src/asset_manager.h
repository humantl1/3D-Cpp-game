#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <map>
#include <string>

#include "entity_manager.h"
#include "texture_manager.h"

class AssetManager {
 public:
  AssetManager(EntityManager* manager);
  ~AssetManager();
  void ClearData();
  void AddTexture(std::string texture_id, const char* file_path);
  SDL_Texture* GetTexture(std::string texture_id);
 private:
  EntityManager* manager_;
  std::map<std::string, SDL_Texture*> textures_;
};

#endif
