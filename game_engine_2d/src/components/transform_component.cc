#if VSCODEWIN
#include "../../ExternalLibs/SDL2/include/SDL.h"
#else
#include <SDL2/SDL.h>
#endif


#include "transform_component.h"
#include "../game.h"

TransformComponent::TransformComponent(
    int pos_x, int pos_y,
    int vel_x, int vel_y,
    int w, int h, int s)
        : position_ {glm::vec2(pos_x, pos_y)},
          velocity_ {glm::vec2(vel_x, vel_y)},
          width_  {w}, height_ {h},
          scale_ {s} {}


void TransformComponent::Initialize() {}

void TransformComponent::Update(float delta_time) {
  position_.x += velocity_.x * delta_time;
  position_.y += velocity_.y * delta_time;
}

void TransformComponent::Render() {
}
