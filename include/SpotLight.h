#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "Light.h"

class Spotlight : virtual public Light
{
private:
 
public:
    float falloff_angle; // The maximum angle the ray should make for it to exist [ANGLE SHOULD BE IN RADIANS]
    float beam_angle;
	Spotlight();
	Spotlight(Transform& transform, glm::vec3 Color, float intensity);
	~Spotlight();

	// Inherited via Light
	virtual glm::vec3 GetShadowVector(glm::vec3 point) const override;
	virtual float GetAbsorbDistance(Ray incoming_ray) const override;

};

inline Spotlight::Spotlight()
{

}

Spotlight::Spotlight(Transform& transform, glm::vec3 Color, float intensity)
{
    this->transform.position = transform.position;
    this->transform.rotation = transform.rotation;
    this->transform.scale = transform.scale;
    this->color = Color;
    this->intensity = intensity;
}

inline Spotlight::~Spotlight()
{
}

float Spotlight::GetAbsorbDistance(Ray incoming_ray) const
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

glm::vec3 Spotlight::GetShadowVector(glm::vec3 point) const
{
    glm::vec3 light_direction = point - transform.position;
    float length_from_light = glm::clamp(0.0f, 1.0f, (float) light_direction.length());

    float angle = acos(glm::dot(transform.rotation, light_direction));

    // // The farther the light 

    length_from_light = glm::smoothstep(0.0f, 1.0f, sqrt(length_from_light));
    // float step = smoothstep(0.0, length_from_light, (falloff_angle - angle)/falloff_angle);
    // return step * light_direction;

    float corrected_falloff = falloff_angle;

    if (angle < beam_angle) {
        return light_direction;
    }
    else if (angle < beam_angle + corrected_falloff) {
        angle = angle - beam_angle;
        float portion = (corrected_falloff - angle) / (corrected_falloff);
        return portion * light_direction;
    }
    return glm::vec3(0, 0, 0);
}


#endif