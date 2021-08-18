#pragma once

#include "entity.h"

class Command {
 public:
   virtual ~Command() {}
   virtual void execute(Entity& entity) = 0;
};

