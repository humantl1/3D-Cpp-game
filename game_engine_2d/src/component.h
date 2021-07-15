#ifndef COMPONENT_H
#define COMPONENT_H

class Entity;

class Component {
 public:
   Entity* owner;
   virtual ~Component() {}
   virtual Initialize() {}
   virtual Update(float delta_time) {}
   virtual Render() {}
 private:
};

#endif
