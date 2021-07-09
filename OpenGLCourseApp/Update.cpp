#include "Update.h"
#include "CommonValues.h"
#include "SDL.h"

Update::Update() : ticks_last_frame{0}, delta_time{0} {}

void Update::RunGameLoop() { FixTime(); }

void Update::FixTime() {
  // wait for 16.6 ms (1/60 sec) to ellapse
  float time_to_wait = kFrameTargetTime - (SDL_GetTicks() - ticks_last_frame);
  
  // delay if rendering is completed before desired fps reached
  if (time_to_wait > 0 && time_to_wait <= kFrameTargetTime) {
    SDL_Delay(time_to_wait);
  }

  // delta time is the difference in ticks from last frame converted to seconds
  delta_time = (SDL_GetTicks() - ticks_last_frame / 1000.0f);

  // clamp delta time
  delta_time = (delta_time > kMaxDeltaTime) ? kMaxDeltaTime : delta_time;

  ticks_last_frame = SDL_GetTicks();
}

Update::~Update() {}
