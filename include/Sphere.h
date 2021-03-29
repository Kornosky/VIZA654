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
    glm::vec3 Right = glm::vec3(1, 0, 0);
public:
	Sphere();
    Sphere(glm::vec3& center, float radius);
	~Sphere();

	// Inherited via VisibleObject
	virtual glm::vec3 getIntersectInfo(const Ray& incoming) const override;
    virtual glm::vec3 getNormalAtPoint(glm::vec3& point) const override;
    virtual glm::vec3 TextureMap(glm::vec3& point) const override;
    virtual glm::vec3 getColor(glm::vec3& point) const override;

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

//Checks to see if it blocks the ray
glm::vec3 Sphere::getIntersectInfo(const Ray& incoming) const
{
    glm::vec3 intersection(0.0f, 0.0f, 0.0f);

    float a = 1.0f;

    float b = glm::dot(incoming.GetDirection(), incoming.GetStartingPoint() - p_c);

    float c = glm::dot(incoming.GetStartingPoint() - p_c, incoming.GetStartingPoint() - p_c) - r * r;

    float delta = b * b - c;
    float distanceToHit = 0.0f;
    float slope = 0.0f;
    float ifIntersect = 0.0f;
    float ifInside = -1.0f;

    if (delta >= 0.0f && b <= 0.0f) {
        distanceToHit = -1.0f * b - sqrt(delta);

        if (distanceToHit <= 0.0f) {
            ifInside = 1.0f;
        }

        ifIntersect = 1.0;
    }
    else {
        distanceToHit = -1.0;
        ifIntersect = -1.0;
    }

    intersection[0] = distanceToHit;
    intersection[1] = ifInside;
    intersection[2] = ifIntersect;

    return intersection;
}

inline glm::vec3 Sphere::getNormalAtPoint(glm::vec3& point) const { return (point - p_c) / r;}

inline glm::vec3 Sphere::TextureMap(glm::vec3& point) const
{
        //# compute the azimuthal angle
        //# -pi < theta <= pi
        //# angle increases clockwise as viewed from above,
        //# which is opposite of what we want, but we'll fix it later.
        //let theta <- arctan2(p.x, p.z)
      
        //# vec is the vector pointing from the sphere's origin (the world origin)
        //# to the point, which will also happen to be exactly equal to the sphere's
        //# radius.
        //let vec <- vector(p.x, p.y, p.z)
        //let radius <- magnitude(vec)
   
        //# compute the polar angle
        //# 0 <= phi <= pi
        //let phi <- arccos(p.y / radius)
   
        //# -0.5 < raw_u <= 0.5
        //let raw_u <- theta / (2 * pi)
     
        //# 0 <= u < 1
        //# here's also where we fix the direction of u. Subtract it from 1,
        //# so that it increases counterclockwise as viewed from above.
        //let u <- 1 - (raw_u + 0.5)
      
        //# we want v to be 0 at the south pole of the sphere,
        //# and 1 at the north pole, so we have to "flip it over"
        //# by subtracting it from 1.
        //let v <- 1 - phi / pi
 
        //return (u, v)
        //end function
  //  return p_c + r * (glm::sin(glm::pi<float>() * v) * glm::sin(2 * glm::p * u * n0) + glm::sin(glm::pi<float>() * v) * glm::cos(2 * glm::pi<float>() * u * n1) + glm::cos(glm::pi<float>() * v * n2));
    return glm::vec3(0,0,0);
}

inline glm::vec3 Sphere::getColor(glm::vec3& point) const
{
    if (m != NULL) {
        float theta = atan2(-1 * (point.z - p_c.z), point.x - p_c.x);      
        float phi = acos(-1 * (point.y - p_c.y) / r);
        float u = (theta + glm::pi<float>()) / (2.0 * glm::pi<float>());
        float v = phi / glm::pi<float>();

        return m->getBilinearColor(u, v);
    }
    return color;
}

inline void Sphere::MoveSphere(Camera_Movement direction, float deltaTime)
{
    float velocity = 1 * deltaTime;
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