#include "transform_component.h"

#include <SDL.h>
#include <algorithm>


TransformComponent::TransformComponent(glm::vec3 position,
                   glm::vec3 velocity,
                   float yaw, float pitch, float roll,
                   float scale,
                   float turn_speed, float move_speed)
  : position_ {position},
    velocity_ {velocity},
    yaw_ {yaw}, pitch_ {pitch}, roll_ {roll},
    scale_ {scale},
    turn_speed_ {turn_speed},
    move_speed_ {move_speed} {}

void TransformComponent::Initialize() {
  CalcRotation();
}

void TransformComponent::Update(float delta_time) {
  position_ += velocity_ * delta_time;
  pitch_ = std::clamp(pitch_, -89.0f, 89.0f);
  CalcRotation();
}

void TransformComponent::CalcRotation() {
  rotation_.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
  rotation_.y = sin(glm::radians(pitch_));
  rotation_.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
  rotation_ = glm::normalize(rotation_);
}

