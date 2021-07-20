#ifndef COMPONENT_H
#define COMPONENT_H

class Entity;

class Component {
 public:
   Entity* owner;
   virtual ~Component() {}
   virtual void Initialize() {}
   virtual void Update(float delta_time) {}
   virtual void Render() {}
 private:
};

#endif
