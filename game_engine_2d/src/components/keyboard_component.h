#ifndef KEYBOARDCOMPONENT_H
#define KEYBOARDCOMPONENT_H

#include <string>

#include "../game.h"
#include "../entity_manager.h"
#include "transform_component.h"
#include "sprite_component.h"

class KeyboardComponent : public Component {
 public:
  std::string up_key_;
  std::string dn_key_;
  std::string lf_key_;
  std::string rt_key_;
  std::string shoot_key_;
  TransformComponent* transform_;
  SpriteComponent* sprite_;

  KeyboardComponent() {}
  KeyboardComponent(std::string up_key, std::string rt_key, std::string dn_key,
                    std::string lf_key, std::string shoot_key);
  std::string GetSDLKeyStringCode(std::string key);
  void Update(float delta_time) override;
  void Initialize() override;
 private:
};

#endif