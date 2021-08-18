#pragma once

#include "input_component.h"
#include "command.h"
#include "flashlight_command.h"

class FlashlightCommand : public Command {
 public:
  virtual void execute(InputComponent& action) {
    action.Flashlight();
  }
};
