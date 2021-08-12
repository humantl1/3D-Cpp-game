#include "animation.h"

Animation::Animation() {}

Animation::Animation(unsigned int index, unsigned int num_frames,
  unsigned int animation_speed) :
    index_ {index}, num_frames_ {num_frames},
    animation_speed_ {animation_speed} {}