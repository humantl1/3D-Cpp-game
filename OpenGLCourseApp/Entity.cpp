#include "entity.h"

#include <iostream>

Entity::Entity(EntityManager& manager) : manager_ {manager},
                                         is_active_ {true} {}

Entity::Entity(EntityManager& manager, std::string name) : manager_ {manager},
                                                           name_ {name},
                                                           is_active_ {true} {}

void Entity::Update(float delta_time) {
  for (auto& component : components_) {
    component->Update(delta_time);
  }
}

void Entity::Render() {
  for (auto& component : components_) {
    component->Render();
  }
}

bool Entity::IsActive() const {
  return this->is_active_;
}

void Entity::ListComponents() {
  for (const auto& pair : component_type_map_) {
    std::cout << "\tComponent<" << pair.first->name() << ">" << std::endl;
  }
}

void Entity::Destroy() {
  this->is_active_ = false;
}
