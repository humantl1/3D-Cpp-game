#pragma once

// TODO: handle all input with this class
class Input
{
 public:
  Input();
  void ProcessInput();
  inline bool GetExitPrompt() { return exit_prompt; }

  ~Input();

 private:
  bool exit_prompt;
};

