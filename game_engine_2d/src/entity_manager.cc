#include "entity_manager.h"
#include "entity.h"
#include <cstdlib>
#include <iostream>

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

Entity& EntityManager::AddEntity(std::string entity_name) {
  Entity* entity = new Entity(*this, entity_name);
  entities_.emplace_back(entity);
  return *entity;
}

std::vector<Entity*> EntityManager::GetEntities() const {
  return entities_;
}

unsigned int EntityManager::GetEntityCount() {
  return entities_.size();
}

void EntityManager::ListEntities() {
  for (const auto& e: entities_) {
    std::cout << "Entity Name: " << e->name_ << std::endl;
    e->ListComponents();
    std::cout << std::endl;
  }
}
