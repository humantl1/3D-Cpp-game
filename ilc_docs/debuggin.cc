SpriteComponent::SpriteComponent(std::string id,
                                 int num_frames,
                                 int animation_speed,
                                 bool has_directions,
                                 bool is_fixed)
                                    : num_frames_       {num_frames},
                                      animation_speed_  {animation_speed},
                                      is_fixed_         {is_fixed},
                                      is_animated_      {true},
                                      animation_index_  {0} {
  if (has_directions) {
    Animation down_animation = Animation(0, num_frames, animation_speed);
    Animation right_animation = Animation(1, num_frames, animation_speed);
    Animation left_animation = Animation(2, num_frames, animation_speed);
    Animation up_animation = Animation(3, num_frames, animation_speed);

    animations_.emplace("down_animation", down_animation);
    animations_.emplace("right_animation", right_animation);
    animations_.emplace("left_animation", left_animation);
    animations_.emplace("up_animation", up_animation);
    this->current_animation_name_ = "down_animation";

/*** Testing: objects in map undefined when referenced ***/
    // TEST: name member variable okay? PASSED
    std::string string_check = "string check";
    std::cout << "initial animation name: " << this->current_animation_name_ << std::endl;

    // TEST: local object okay? PASSED
    std::cout << "down animation frames: " <<
      down_animation.num_frames_ << std::endl;
      std::cout << std::endl;

    // TEST: map key/value pairs okay?
    // PASSED (key is correct string & val correct type)
    std::map<std::string, Animation>::iterator itr;
    std::cout << "animations_ map [key, value]: " << std::endl;
    for (itr = animations_.begin(); itr != animations_.end(); ++itr) {
      std::cout << "[" << itr->first << ", " << typeid(itr->second).name() << "]" << std::endl;
    }
    std::cout << std::endl;

    // TEST: compare local and map object sizes? PASSED (and map element IS accessible by key)
    std::cout << "Size of local: " << sizeof(down_animation) << std::endl;
    std::cout << "Size of map element: " << sizeof(animations_["down_animation"]) << std::endl;
    std::cout << std::endl;

    // TEST: compare memory locations? FAILED: map animation undefined
    // PROBLEM: Animation basic constructor explicitly declared but not defined
    // PASSED! SOLUTION: define default constructor
    std::cout << "Local Animation memory location: " << &down_animation << std::endl;
    std::cout << "Map Animation memory location: " << &animations_["down_animation"] << std::endl;
    std::cout << std::endl;

    // TEST: map contains valid object? FAILED: undefined reference!
    // PASSED! SOLUTION: solved by defining default constructor
    std::cout << "animation[0] frames: " <<
      animations_["down_animation"].num_frames_ << std::endl;

/*** End Testing ***/

  } else {
    Animation single_animation = Animation(0, num_frames, animation_speed);
    animations_.emplace("single_animation", single_animation);
    this->current_animation_name_ = "single_animation";
    }

  Play(this->current_animation_name_);

  SetTexture(id);
}

void SpriteComponent::Play(std::string animation_name) {

/*** Testing ***/
  // TEST: passed name variable okay? PASSED
  std::cout << "passed animation name: " << animation_name << std::endl;
  // TEST: map contains elements? PASSED
  std::cout << "animation map size: " << animations_.size() << std::endl;
/*** End Testing ***/

  // Solved! BUGGED no reference to animation
  num_frames_= animations_[animation_name].num_frames_;
  animation_index_ = animations_[animation_name].index_;
  animation_speed_ = animations_[animation_name].animation_speed_;
  current_animation_name_ = animation_name;
}