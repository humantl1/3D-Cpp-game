#pragma once

#include <string>
#include <vector>
#include <map>
#include <typeinfo>

#include "component.h"

class EntityManager;

class Entity {
 public:
  std::string name_;

 private:
  EntityManager& manager_;
  bool is_active_;
  std::vector<Component*> components_;
  std::map<const std::type_info*, Component*> component_type_map_;

 public:
  Entity(EntityManager& manager);
  Entity(EntityManager& manager, std::string name);
  void Update(float delta_time);
  void Render();
  bool IsActive() const;
  void ListComponents();
  void Destroy();

  template<typename T, typename... TArgs>
  T& AddComponent(TArgs&&... args) {
    T* new_component(new T(std::forward<TArgs>(args)...));
    new_component->owner_ = this;
    components_.emplace_back(new_component);
    component_type_map_[&typeid(*new_component)] = new_component;
    new_component->Initialize();
    return *new_component;
  }

  template<typename T>
  T* GetComponent() {
    return static_cast<T*>(component_type_map_[&typeid(T)]);
  }

  template <typename T>
  bool HasComponent() const {
    return component_type_map_.count(&typeid(T));
  }

 private:
};
