#include "entity.h"

Entity::Entity(EntityManager& manager): manager_ {manager} {
  this->is_active_ = true;
}

Entity::Entity(EntityManager& manager, std::string name): manager_ {manager}, name_{name} {
  this->is_active_ = true;
}

void Entity::Update(float delta_time) {
  for (auto& component: components_) {
    component->Update(delta_time);
  }
}

void Entity::Render() {
  for (auto& component: components_) {
    component->Render();
  }
}

void Entity::Destroy() {
  this->is_active_ = false;
}

bool Entity:: IsActive() const {
  return this->is_active_;
}
