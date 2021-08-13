#pragma once

#include "glm/glm.hpp"

#include "entity_manager.h"

class TransformComponent : public Component{
 public:
  glm::vec3 position_;
  glm::vec3 velocity_;
  int width_;
  int height_;
  int depth_;
  int scale_;

  TransformComponent(int pos_x, int pos_y, int pos_z,
                     int vel_x, int vel_y, int vel_z,
                     int w, int h, int d, int s);
  void Initialize() override {}
  void Update(float delta_time) override;
  void Render() override {}

 private:
};
