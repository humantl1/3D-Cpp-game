#include "transform_component.h"

#include <SDL.h>


  TransformComponent::TransformComponent(int pos_x, int pos_y, int pos_z,
                     int vel_x, int vel_y, int vel_z,
                     int w, int h, int d, int s)
    : position_ {glm::vec3(pos_x, pos_y, pos_x)},
      velocity_ {glm::vec3(vel_x, vel_y, vel_z)},
      width_ {w}, height_ {h}, depth_ {d}, scale_ {d} {}

  void TransformComponent::Update(float delta_time) {
    position_.x += velocity_.x * delta_time;
    position_.y += velocity_.y * delta_time;
    position_.z += velocity_.z * delta_time;
  }
