#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "Light.h"

class PointLight : virtual public Light
{
private:

public:
	PointLight();
	PointLight(glm::vec3 position, glm::vec3 Color, float intensity);
	~PointLight();

	// Inherited via Light
	virtual glm::vec3 GetShadowVector(glm::vec3 point) const override;
	virtual float GetAbsorbDistance(Ray incoming_ray) const override;

};

inline PointLight::PointLight()
{
}

PointLight::PointLight(glm::vec3 position, glm::vec3 Color, float intensity)
{
	transform.position = position;
	this->color = Color;
	this->intensity = intensity;
}

inline PointLight::~PointLight()
{
}

float PointLight::GetAbsorbDistance(Ray incoming_ray) const
{
    glm::vec3 intersection(0.0f, 0.0f, 0.0f);

    float b = glm::dot(incoming_ray.GetDirection(), incoming_ray.GetStartingPoint() - transform.position);

    float c = glm::dot(incoming_ray.GetStartingPoint() - transform.position, incoming_ray.GetStartingPoint() - transform.position) - intensity * intensity;

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

    if (ifIntersect > 0) //if intersect then check to see how far it intersects for
    {
        incoming_ray = Ray(incoming_ray.GetPoint(distanceToHit), incoming_ray.GetDirection());
        b = glm::dot(incoming_ray.GetDirection(), incoming_ray.GetStartingPoint() - transform.position);

        c = glm::dot(incoming_ray.GetStartingPoint() - transform.position, incoming_ray.GetStartingPoint() - transform.position) - intensity * intensity;

        delta = b * b - c;


        //If inside the sphere
        if (delta <= 0.0f && b >= 0.0f) {
            distanceToHit = -1.0f * b - sqrt(delta);
            return distanceToHit;
        }
    }


    return 0;
}

glm::vec3 PointLight::GetShadowVector(glm::vec3 point) const
{
	return glm::normalize(transform.position - point);
}


#endif