#pragma once
class Update {
 public:
  Update();
  void RunGameLoop();
  inline float GetDeltaTime() { return delta_time; }

  ~Update();
 private:
  int ticks_last_frame;
  float delta_time;

  void FixTime();
};
