#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <string>
#include <map>

#include "component.h"

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

  template<typename T, typename... TArgs>
  T& AddComponent(TArgs&&... args) {
    // std::forwared required for passing variable T args
    T* new_component(new T(std::forward<TArgs>(args)...));
    new_component->owner = this;
    components_.emplace_back(new_component);
    component_type_map_[&typeid(*new_component)] = new_component;
    new_component->Initialize();
    return *new_component;
  }

  template <typename T>
  T* GetComponent() {
    return static_cast<T*>(component_type_map_[&typeid(T)]);
  }

 private:
  EntityManager& manager_;
  bool is_active_;
  std::vector<Component*> components_;
  std::map<const std::type_info*, Component*> component_type_map_;
};

#endif
