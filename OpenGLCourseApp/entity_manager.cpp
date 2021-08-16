#include "entity_manager.h"

#include <iostream>

void EntityManager::Update(float delta_time) {
  for (auto& entity : entities_) {
    entity->Update(delta_time);
  }
}

void EntityManager::Render() {
  for (auto& entity : entities_) {
    entity->Render();
  }
}

bool EntityManager::HasNoEntities() {
  return entities_.size() == 0;
}

unsigned int EntityManager::GetEntityCount() {
  return entities_.size();
}

void EntityManager::ListEntities() {
  for (const auto& entity : entities_) {
    std::cout << "Entity Name: " << entity->name_ << std::endl;
    entity->ListComponents();
    std::cout << std::endl;
  }
}

Entity& EntityManager::AddEntity(std::string entity_name) {
  Entity* entity = new Entity(*this, entity_name);
  entities_.emplace_back(entity);
  return *entity;
}

// Danger: returns copy of vector. Pointers still point to EntityManager scope 
std::vector<Entity*> EntityManager::GetEntities() const {
  return entities_;
}

void EntityManager::ClearData() {
  for (auto& entity : entities_) {
    entity->Destroy();
  }
}
