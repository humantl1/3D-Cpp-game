#pragma once

#include <map>
#include <string>
#include <GL/glew.h>

// use asset manager as a user interface for textures, models, and materials
class AssetManager {
 public:
   AssetManager(); // EntityManager parameter?
   ~AssetManager(); // Ensure all assets are deleted
   void AddTexture(); // call Texture manager to load texture
   GLuint GetTexture(std::string texture_id); // Call texture mng UseTexture
   // Call Texture manager to clear textures
   void ClearTexture(std::string); // clear single texture
   void ClearTextures(); // clear all textures
  
 private:
  std::map<std::string, GLuint > textures_; // <human readable, OpenGL ID>
};
