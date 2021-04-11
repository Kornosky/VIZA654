#ifndef PLANE_H
#define PLANE_H

#include "glm/glm.hpp"
#include <Transform.h>
#include <VisibleObject.h>

class Plane : public VisibleObject {
private:

    Transform transform; //Not used yet? I want to use it
    glm::vec3 n_0; // Normal to the plane
    glm::vec3 n_1, n_2; // Two orthogonal vectors on the plane
    glm::vec3 p_i; // A given point on the plane
    glm::vec3 c_0; // Color of the plane

    float sx, sy;
public:
    Plane();
    Plane(glm::vec3 cornerLocation, float width, float height);
    Plane(glm::vec3 point, glm::vec3 normal);
    ~Plane();

    // Inherited via VisibleObject
    virtual glm::vec3 getIntersectInfo(const Ray& incoming) const override;
    virtual glm::vec3 getNormalAtPoint(glm::vec3& point) const override;
    virtual glm::vec3 TextureMap(glm::vec3& point) const override;
    virtual glm::vec3 getColor(glm::vec3& point) const override;
};

Plane::Plane() {}

inline Plane::Plane(glm::vec3 cornerLocation, glm::vec3 normal)
{
    n_0 = normal;
    n_0 = glm::normalize(n_0); 
    p_i = cornerLocation;
    glm::vec3 v_temp; // Temporary variable which is not parallel to the normal
    if (n_0.z >= 1.0 || n_0.z <= -1.0) {
        v_temp = glm::vec3(0.0, n_0.z, 0.0);
    }
    else {
        v_temp = glm::vec3(-1.0f * n_0.y, n_0.x, n_0.z);
    }
    v_temp = glm::normalize(v_temp);

    n_1 = cross(n_0, v_temp);
    n_2 = cross(n_0, n_1);
    n_1 = glm::normalize(n_1);
    n_2 = glm::normalize(n_2);

    sx = 20.0f;
    sy = 20.0f;
}

Plane::~Plane() {}


//Checks to see if it blocks the ray
glm::vec3 Plane::getIntersectInfo(const Ray& incoming) const
{
    glm::vec3 intersection(0.0f, 0.0f, 0.0f);

    float slope = glm::dot(incoming.GetDirection(), n_0);
    float t = 0.0f;
    float ifIntersect = 0.0f;

    bool ifAngled = false;

    if (abs(slope) > 0.000001f) {
        t = glm::dot(p_i - incoming.GetStartingPoint(), n_0) / slope;
        ifAngled = true;
    }

    if (t >= 0.0f && ifAngled) {
        ifIntersect = 1.0f;
    }
    else {
        ifIntersect = -1.0f;
    }

    intersection[0] = t;
    intersection[1] = -1;
    intersection[2] = ifIntersect;

    return intersection;
}

inline glm::vec3 Plane::getNormalAtPoint(glm::vec3& point) const
{
    return n_0;
}

inline glm::vec3 Plane::TextureMap(glm::vec3& point) const
{
    return glm::vec3();
}

inline glm::vec3 Plane::getColor(glm::vec3& point) const
{
    if (mat != NULL) {
        float u = dot(n_1, point - p_i) / 20.0f;
        float v = dot(n_2, point - p_i) / 20.0f;

        return mat->getBilinearColor(u, v);
    }
    return color;
}

#endif