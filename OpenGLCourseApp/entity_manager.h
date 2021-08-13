#pragma once

#include <vector>
#include <string>

#include "entity.h"

class EntityManager {
 public:
  void Update(float delta_time);
  void Render();
  bool HasNoEntities();
  unsigned int GetEntityCount();
  void ListEntities();
  Entity& AddEntity(std::string entity_name);
  std::vector<Entity*> GetEntities() const;
  void ClearData();

 private:
  std::vector<Entity*> entities_;
};
