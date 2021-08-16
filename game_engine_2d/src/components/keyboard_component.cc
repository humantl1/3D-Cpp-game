#include "keyboard_component.h"

KeyboardComponent::KeyboardComponent(std::string up_key, std::string rt_key,
                                     std::string dn_key, std::string lf_key,
                                     std::string shoot_key)
    : up_key_ {GetSDLKeyStringCode(up_key)},
      rt_key_ {GetSDLKeyStringCode(rt_key)},
      dn_key_ {GetSDLKeyStringCode(dn_key)},
      lf_key_ {GetSDLKeyStringCode(lf_key)},
      shoot_key_ {GetSDLKeyStringCode(shoot_key)} {}

std::string KeyboardComponent::GetSDLKeyStringCode(std::string key) {
  if (key.compare("up") == 0) return "1073741906";
  if (key.compare("down") == 0) return "1073741905";
  if (key.compare("left") == 0) return "1073741904";
  if (key.compare("right") == 0) return "1073741903";
  if (key.compare("shoot") == 0) return "32";
  return std::to_string(static_cast<int>(key[0]));
}

void KeyboardComponent::Initialize() {
  transform_ = owner->GetComponent<TransformComponent>();
  sprite_ = owner->GetComponent<SpriteComponent>();
}

void KeyboardComponent::Update(float delta_time) {
  if (Game::event_.type == SDL_KEYDOWN) {
    std::string key_code = std::to_string(Game::event_.key.keysym.sym);

    if (key_code.compare(up_key_) == 0) {
      transform_->velocity_.y = -40;
      transform_->velocity_.x = 0;
      sprite_->Play("up_animation");
    }
    if (key_code.compare(rt_key_) == 0) {
      transform_->velocity_.y = 0;
      transform_->velocity_.x = 40;
      sprite_->Play("right_animation");
    }
    if (key_code.compare(dn_key_) == 0) {
      transform_->velocity_.y = 40;
      transform_->velocity_.x = 0;
      sprite_->Play("down_animation");
    }
    if (key_code.compare(lf_key_) == 0) {
      transform_->velocity_.y = 0;
      transform_->velocity_.x = -40;
      sprite_->Play("left_animation");
    }
    if (key_code.compare(shoot_key_) == 0) {
      // TODO
    }
  }

  if (Game::event_.type == SDL_KEYUP) {
    std::string key_code = std::to_string(Game::event_.key.keysym.sym);

    if (key_code.compare(up_key_) == 0) {
      transform_->velocity_.y = 0;
    }
    if (key_code.compare(rt_key_) == 0) {
      transform_->velocity_.x = 0;
    }
    if (key_code.compare(dn_key_) == 0) {
      transform_->velocity_.y = 0;
    }
    if (key_code.compare(lf_key_) == 0) {
      transform_->velocity_.x = 0;
    }
    if (key_code.compare(shoot_key_) == 0) {
      // TODO
    }

  }
}