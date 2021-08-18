#pragma once

// TODO: handle all input with this class
class InputManager
{
 public:
  InputManager();
  void ProcessInput();
  inline bool GetExitPrompt() const { return exit_prompt_; }
  inline bool GetInputEvent() const {return input_event_;}
  ~InputManager();
  
 private:
  bool exit_prompt_;
  bool input_event_;
  SDL_Event event_;
};
