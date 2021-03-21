
#ifndef VISIBLEOBJECTH
#define VISIBLEOBJECTH

#include <glm//glm.hpp>
#include "Ray.h"
#include <vector>
//#include "Material.h"
//#include "Helpers.h"

class VisibleObject
{
private:
    //These vectors serve just to hold all of the data for all of your vertices
    vector<glm::vec3> positions;
    vector<glm::vec3> normals;
    vector<glm::vec3> colors;
    //This struct holds indices to the vectors above
    struct Vertex
    {
        int position;
        int normal;
        int color;
    };

    //Holds a list of all of your vertices, but allows you to pair different positions with different normals, etc...
    vector<Vertex> vertices;

    //Holds a list of indices from vertices that are your points to draw.
    vector<int> drawPoints;
public:
     VisibleObject();
     ~VisibleObject();
     glm::vec3  color;
    // inline virtual void setMaterial(Material& material) { m = &material; }

    // The function will return a glm::vec3 with x : Parameter t, y : slope of hit, z : yet to be decided (-1)
    // Thought: We could also return the normal vector
     virtual glm::vec3 getIntersectInfo(const Ray& incoming) const = 0;

     virtual glm::vec3 getNormalAtPoint(glm::vec3& point) const;

     virtual glm::vec3 getColor(glm::vec3& point) const;

  //   virtual int getTypeID() const { return PLANE_TYPE_ID; }

    // For traingular meshes
     virtual glm::vec3 getNormalAtPoint(glm::vec3& point, int id_triangle) const { return glm::vec3(0.0f, 0.0f, 0.0f); };
     virtual glm::vec3 getColor(glm::vec3& point, int id_triangle) const;

     virtual void update(float t);

protected:
  //  Material* m = NULL; // Material of the object
};

 VisibleObject::VisibleObject() {}

 VisibleObject::~VisibleObject() {}

 glm::vec3 VisibleObject::getNormalAtPoint(glm::vec3& point) const { return glm::vec3(0.0f, 0.0f, 0.0f); }

 inline glm::vec3 VisibleObject::getColor(glm::vec3& point, int id_triangle) const
 {
     return glm::vec3();
 }
 glm::vec3 VisibleObject::getColor(glm::vec3& point) const { return color; }

 void VisibleObject::update(float t) {
}

#endif