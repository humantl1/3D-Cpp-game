#ifndef ANIMATION_H
#define ANIMATION_H

class Animation {
 public:
  unsigned int index_;
  unsigned int num_frames_;
  unsigned int animation_speed_;

  Animation();
  Animation(unsigned int index, unsigned int num_frames, unsigned int animation_speed);
};

#endif