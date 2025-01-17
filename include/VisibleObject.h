
#ifndef VISIBLEOBJECTH
#define VISIBLEOBJECTH

#include "Material.h"
#include "glm/glm.hpp"
#include "Ray.h"
#include <vector>

class VisibleObject
{
private:

public:
     VisibleObject();
     ~VisibleObject();
     glm::vec3  color = glm::vec3(255, 0, 255);
     inline virtual void setMaterial(Material& material) { mat = &material; }

     bool isPrimative = true;
    // The function will return a glm::vec3 with x : Parameter t, y : slope of hit, z : yet to be decided (-1)
    // Thought: We could also return the normal vector
     virtual glm::vec3 getIntersectInfo(const Ray& incoming) const = 0;

     Material* mat = NULL; // Material of the object
     virtual glm::vec3 getColor(glm::vec3& point) const;

  //   virtual int getTypeID() const { return PLANE_TYPE_ID; }
     enum PrimType { SPHERE, PLANE, MESH};
    // For traingular meshes
         // For traingular meshes
     virtual glm::vec3 getNormalAtPoint(glm::vec3& point, int id_triangle) const { return glm::vec3(0.0f, 0.0f, 0.0f); };
     virtual glm::vec3 getColor(glm::vec3& point, int id_triangle) const { return glm::vec3(0.0f, 0.0f, 0.0f); };
     virtual glm::vec3 getNormalAtPoint(glm::vec3& point) const { return glm::vec3(0.0f, 0.0f, 0.0f); };
     virtual glm::vec3 TextureMap(glm::vec3& point) const;
     virtual void update(float t);

     virtual int getTypeID() const { return PLANE; }

protected:
   
};

 VisibleObject::VisibleObject() {}

 VisibleObject::~VisibleObject() {}


 inline glm::vec3 VisibleObject::TextureMap(glm::vec3& point) const
 {
     return glm::vec3();
 }
 glm::vec3 VisibleObject::getColor(glm::vec3& point) const { return color; }

 void VisibleObject::update(float t) {
}

#endif