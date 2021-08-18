#pragma once

#include "glm/glm.hpp"

#include "entity_manager.h"

class TransformComponent : public Component{
 public:
  glm::vec3 position_;
  glm::vec3 velocity_;
  glm::vec3 rotation_;
  float yaw_;
  float pitch_;
  float roll_;
  //int width_;
  //int height_;
  //int depth_;
  float scale_;
  float turn_speed_;
  float move_speed_;

  TransformComponent(glm::vec3 position,
                     glm::vec3 velocity,
                     float yaw, float pitch, float roll,
                     float scale,
                     float turn_speed, float move_speed);
  void Initialize() override;
  void Update(float delta_time) override;
  void Render() override {}
  void CalcRotation();

 private:
};
