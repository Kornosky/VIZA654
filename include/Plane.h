#ifndef PRIMATIVES_H
#define PRIMATIVES_H

#include "glm/glm.hpp"
#include <Transform.h>
#include <VisibleObject.h>

class Plane : public VisibleObject {
private:
	
	Transform transform; //Not used yet? I want to use it
    float width;
    float height;
    glm::vec3 cornerLocation;
    glm::vec3 n_0;
    glm::vec3 p_i;
public:
	Plane();
    Plane(glm::vec3& cornerLocation, float width, float height);
    Plane(glm::vec3& point, glm::vec3& normal);
	~Plane();

	// Inherited via VisibleObject
	virtual glm::vec3 getIntersectInfo(const Ray& incoming) const override;
};

Plane::Plane() {}

inline Plane::Plane(glm::vec3& cornerLocation, float width, float height)
{
    this->cornerLocation = cornerLocation;
    this->width = width;
    this->height = height;
}

inline Plane::Plane(glm::vec3& point, glm::vec3& normal) : p_i(point), n_0(glm::normalize(normal)) {}

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
        t = dot(p_i - incoming.GetStartingPoint(), n_0) / slope;
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

#endif