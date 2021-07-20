#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <string>
#include <memory>

#include "entity_manager.h"
#include "component.h"

class Component;
class EntityManager;

class Entity {
 public:
  std::string name_;

  Entity(EntityManager& manager);
  Entity(EntityManager& manager, std::string name);
  void Update(float delta_time);
  void Render();
  void Destroy();
  bool IsActive() const;

 private:
  EntityManager& manager_;
  bool is_active_;
  std::vector<std::unique_ptr<Component>> components_;
};

#endif
