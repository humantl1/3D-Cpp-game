#pragma once

#include "entity_manager.h"

class Update {
 public:
  Update();
  void RunGameLoop(EntityManager& entity_manager);
  inline float GetDeltaTime() { return delta_time; }

  ~Update();
 private:
  int ticks_last_frame;
  float delta_time;

  void FixTime();
};
