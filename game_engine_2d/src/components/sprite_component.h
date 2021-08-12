#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#if VSCODEWIN
#include "../../ExternalLibs/SDL2/include/SDL.h"
#else
#include <SDL2/SDL.h>
#endif

#include "../texture_manager.h"
#include "../asset_manager.h"
#include "transform_component.h"
#include "../animation.h"

class SpriteComponent : public Component {
 public:
  SpriteComponent(const char* file_path);
  SpriteComponent(std::string id, int num_frames, int animation_speed,
      bool has_directions, bool is_fixed);

  SDL_RendererFlip sprite_flip_;
  void SetTexture(std::string asset_texture_id);
  void Initialize() override;
  void Update(float delta_time) override;
  void Render() override;
  void Play(std::string animation_name);

 private:
  TransformComponent* transform_;
  SDL_Texture* texture_;
  SDL_Rect source_rectangle_;
  SDL_Rect destination_rectangle_;
  bool is_animated_;
  int num_frames_;
  int animation_speed_;
  bool is_fixed_;
  std::map<std::string, Animation> animations_;
  std::string current_animation_name_;
  unsigned int animation_index_;
};

#endif