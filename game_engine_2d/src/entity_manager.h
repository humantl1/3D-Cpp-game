#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <memory>
#include <vector>

#include "component.h"
class Entity;

class EntityManager {
 public:
  void ClearData();
  void Update(float delta_time);
  void Render();
  bool HasNoEntities();
  Entity& AddEntity(std::string entity_name);
  std::vector<Entity*> GetEntities() const;
  unsigned int GetEntityCount();
 private:
  std::vector<Entity*> entities_;
};

#endif
