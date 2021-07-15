#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <memory>
#include <vector>

#include "entity.h"
#include "component.h"

class EntityManager {
 public:
  void ClearData();
  void Update(float delta_time);
  void Render();
  bool HasNoEntities();
  std::unique_ptr<Entity> AddEntity(std::string entity_name);
  std::vector<std::unique_ptr<Entity>> GetEntities() const;
  unsigned int GetEntityCount();
 private:
  std::vector<std::unique_ptr<Entity>> entities_;
};

#endif
