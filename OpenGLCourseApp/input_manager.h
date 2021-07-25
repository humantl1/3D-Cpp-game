#pragma once

// TODO: handle all input with this class
class InputManager
{
 public:
  InputManager();
  void ProcessInput();
  inline bool GetExitPrompt() { return exit_prompt; }

  ~InputManager();

 private:
  bool exit_prompt;
};

