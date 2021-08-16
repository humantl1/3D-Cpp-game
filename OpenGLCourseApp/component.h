#pragma once

class Entity;

class Component {
 public:
  Entity* owner_;
  virtual void Initialize() {}
  virtual void Update(float delta_time) {}
  virtual void Render() {}
  virtual ~Component() {}
 private:
};
