#ifndef PRIMATIVES_H
#define PRIMATIVES_H

#include "glm/glm.hpp"
#include <Transform.h>
#include <VisibleObject.h>

class Sphere : public VisibleObject {
private:
	
	Transform transform; //Not used yet? I want to use it
    float r;
    glm::vec3 p_c;
    //Movement
    glm::vec3 Front = glm::vec3(0, 1, 0);
    glm::vec3 Right = glm::vec3(0, 0, 1);
public:
	Sphere();
    Sphere(glm::vec3& center, float radius);
	~Sphere();

	// Inherited via VisibleObject
	virtual glm::vec3 getIntersectInfo(const Ray& incoming) const override;
    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void MoveSphere(Camera_Movement direction, float deltaTime);
};

Sphere::Sphere() {}

inline Sphere::Sphere(glm::vec3& center, float radius)
{
    p_c = center;
    r = radius;
}

Sphere::~Sphere() {}
//compute the roots of quadratic equation
bool solveQuadratic(const float& a, const float& b, const float& c, float& x0, float& x1)
{
    float discr = b * b - 4 * a * c;
    if (discr < 0) return false;
    else if (discr == 0) {
        x0 = x1 = -0.5 * b / a;
    }
    else {
        float q = (b > 0) ?
            -0.5 * (b + sqrt(discr)) :
            -0.5 * (b - sqrt(discr));
        x0 = q / a;
        x1 = c / q;
    }

    return true;
}

//Checks to see if it blocks the ray
glm::vec3 Sphere::getIntersectInfo(const Ray& incoming) const
{
    //glm::vec3 intersection(0.0f, 0.0f, 0.0f);

    //float a = 1.0f;

    //float b = glm::dot(incoming.GetDirection(), incoming.GetStartingPoint() - p_c);

    //float c = glm::dot(incoming.GetStartingPoint() - p_c, incoming.GetStartingPoint() - p_c) - r * r;

    //float delta = b * b - c;
    //float distanceToHit = 0.0f;
    //float slope = 0.0f;
    //float ifIntersect = 0.0f;
    //float ifInside = -1.0f;

    //if (delta >= 0.0f && b <= 0.0f) {
    //    distanceToHit = -1.0f * b - sqrt(delta);

    //    if (distanceToHit <= 0.0f) {
    //        ifInside = 1.0f;
    //    }

    //    ifIntersect = 1.0;
    //}
    //else {
    //    distanceToHit = -1.0;
    //    ifIntersect = -1.0;
    //}

    //intersection[0] = distanceToHit;
    //intersection[1] = ifInside;
    //intersection[2] = ifIntersect;

    //return intersection;
    //https://www.scratchapixel.com/code.php?id=10&origin=/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes
    float t0, t1; // solutions for t if the ray intersects 
    glm::vec3 intersection(0.0f, 0.0f, 0.0f);

        // analytic solution
    glm::vec3 L = incoming.GetStartingPoint() - p_c;
    float a = glm::dot(incoming.GetDirection(), incoming.GetDirection());
    float b = 2 * glm::dot(incoming.GetDirection(), L);
    float c = glm::dot(L,L) - (r*r);
    if (!solveQuadratic(a, b, c, t0, t1)) return intersection;

    if (t0 > t1) std::swap(t0, t1);

    if (t0 < 0) {
        t0 = t1; // if t0 is negative, let's use t1 instead 
        if (t0 < 0) return intersection; // both t0 and t1 are negative 
    }

    intersection[0] = t0;
  //  intersection[1] = ifInside;
    intersection[2] = 1; //ifIntersect

     return intersection;
}

inline void Sphere::MoveSphere(Camera_Movement direction, float deltaTime)
{
    float velocity = 100 * deltaTime;
    if (direction == FORWARD)
        p_c += Front * velocity;
    if (direction == BACKWARD)
        p_c -= Front * velocity;
    if (direction == LEFT)
        p_c -= Right * velocity;
    if (direction == RIGHT)
        p_c += Right * velocity;

}

#endif