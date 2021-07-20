#include "entity_manager.h"
#include <atomic>
#include <cstdlib>
#include <memory>

void EntityManager::ClearData() {
  for (auto& entity : entities_) {
    entity->Destroy();
  }
}

bool EntityManager::HasNoEntities() {
  return entities_.size() == 0;
}

void EntityManager::Update(float delta_time) {
  for (auto& entity : entities_) {
    entity->Update(delta_time);
  }
}

void EntityManager::Render() {
  for (auto& entity : entities_)
    entity->Render();
}

std::unique_ptr<Entity> EntityManager::AddEntity(std::string entity_name) {
  std::unique_ptr<Entity> entity(new Entity(*this, entity_name));
  entities_.emplace_back(entity);
  return entity;
}

std::vector<std::unique_ptr<Entity>> EntityManager::GetEntities() const {
  return entities_;
}

unsigned int EntityManager::GetEntityCount() {
  return entities_.size();
}

