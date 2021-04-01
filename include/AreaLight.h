#ifndef AREALIGHT_H
#define AREALIGHT_H

#include "Light.h"

class AreaLight : virtual public Light
{
private:
    glm::vec3 c_0; // Position of the origin of the area
    glm::vec3 c_c; // Position of the center of the area
    glm::vec3 n_0, n_1; // Normal to the area
    glm::vec3 a_n, a_up; // The up vector for the area
    float w, h;   // Width and height of the area 

    glm::vec3 current_position; // The position of the current sample of the area light
public:
	AreaLight();
	AreaLight(glm::vec3 position, glm::vec3 Color, float intensity);
    AreaLight(glm::vec3& position, glm::vec3& normal, glm::vec3& up, float width, float height);
	~AreaLight();

    virtual void setRandomSamplePosition() override;
	// Inherited via Light
	virtual glm::vec3 GetShadowVector(glm::vec3 point) const override;
	virtual float GetAbsorbDistance(Ray incoming_ray) const override;

};

inline AreaLight::AreaLight()
{
}

AreaLight::AreaLight(glm::vec3 position, glm::vec3 Color, float intensity)
{
	transform.position = position;
	this->color = Color;
	this->intensity = intensity;
    hasSamples = true;
}

inline AreaLight::~AreaLight()
{
}

float AreaLight::GetAbsorbDistance(Ray incoming_ray) const
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

glm::vec3 AreaLight::GetShadowVector(glm::vec3 point) const
{
    glm::vec3 p = glm::normalize(point - current_position);
    
    return p;
}


AreaLight::AreaLight(glm::vec3& position, glm::vec3& normal, glm::vec3& up, float width, float height)
    : c_c(position), a_n(normal), a_up(up), w(width), h(height) {
    a_n = glm::normalize(a_n); 
    a_up = glm::normalize(a_up);

    n_0 = glm::normalize(cross(a_n, a_up));

    n_1 = glm::normalize(cross(n_0, a_n));

    c_0 = c_c - (w / 2.0f) * n_0 - (h / 2.0f) * n_1;
    current_position = c_c;
}



void AreaLight::setRandomSamplePosition() {
    float x = rand() * w;
    float y = rand() * h;

    current_position = c_0 + x * n_0 + y * n_1;
}
#endif