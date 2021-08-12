#include "sprite_component.h"
#include <string>
#include <iostream>
#include <map>

SpriteComponent::SpriteComponent(const char* filePath) :
    animation_index_ {0},
    sprite_flip_ {SDL_FLIP_NONE},
    is_animated_ {false},
    num_frames_ {0},
    is_fixed_ {0} {
  SetTexture(filePath);
}

SpriteComponent::SpriteComponent(std::string id,
                                 int num_frames,
                                 int animation_speed,
                                 bool has_directions,
                                 bool is_fixed)
                                    : num_frames_       {num_frames},
                                      animation_speed_  {animation_speed},
                                      is_fixed_         {is_fixed},
                                      is_animated_      {true},
                                      animation_index_  {0} {
  if (has_directions) {
    Animation down_animation = Animation(0, num_frames, animation_speed);
    Animation right_animation = Animation(1, num_frames, animation_speed);
    Animation left_animation = Animation(2, num_frames, animation_speed);
    Animation up_animation = Animation(3, num_frames, animation_speed);

    animations_.emplace("down_animation", down_animation);
    animations_.emplace("right_animation", right_animation);
    animations_.emplace("left_animation", left_animation);
    animations_.emplace("up_animation", up_animation);
    this->current_animation_name_ = "down_animation";
  } else {
    Animation single_animation = Animation(0, num_frames, animation_speed);
    animations_.emplace("single_animation", single_animation);
    this->current_animation_name_ = "single_animation";
    }

  Play(this->current_animation_name_);

  SetTexture(id);
}

void SpriteComponent::Initialize() {
  transform_ = owner->GetComponent<TransformComponent>();
  source_rectangle_.x = 0;
  source_rectangle_.y = 0;
  source_rectangle_.w = transform_->width_;
  source_rectangle_.h = transform_->height_;
}

void SpriteComponent::SetTexture(std::string asset_texture_id) {
  texture_ = Game::asset_manager_->GetTexture(asset_texture_id);
}

void SpriteComponent::Play(std::string animation_name) {
  num_frames_= animations_[animation_name].num_frames_;
  animation_index_ = animations_[animation_name].index_;
  animation_speed_ = animations_[animation_name].animation_speed_;
  current_animation_name_ = animation_name;
}

void SpriteComponent::Update(float delta_time) {
  if (is_animated_) {
    source_rectangle_.x = source_rectangle_.w *
      static_cast<int>((SDL_GetTicks() / animation_speed_) % num_frames_);
  }
  source_rectangle_.y = animation_index_ * transform_->height_;

  destination_rectangle_.x = static_cast<int>(transform_->position_.x);
  destination_rectangle_.y = static_cast<int>(transform_->position_.y);
  destination_rectangle_.w = transform_->width_ * transform_->scale_;
  destination_rectangle_.h = transform_->height_ * transform_->scale_;
}

// call public Texture Manager method Draw()
// which simply calls SDL_RenderCopyEx()
void SpriteComponent::Render() {
  TextureManager::Draw(texture_, source_rectangle_, destination_rectangle_,
  sprite_flip_);
}

