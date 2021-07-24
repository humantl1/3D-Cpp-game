#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include "../entity_manager.h"
#include "../../lib/glm/glm.hpp"

class TransformComponent : public Component {
 public:
  glm::vec2 position_;
  glm::vec2 velocity_;
  int width_;
  int height_;
  int scale_;

  TransformComponent(
      int pos_x, int pos_y, // positional data
      int vel_x, int vel_y, // velocity data : included in transform 
                            // (not rigidbody) for simplicity
      int w, int h, int s); // size and scale data
  void Initialize() override;
  void Update(float delta_time) override;
  void Render() override;
};

#endif
