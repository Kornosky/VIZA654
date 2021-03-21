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
	virtual glm::vec3 GetLightColorAtPoint(glm::vec3 point) const override;
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

glm::vec3 PointLight::GetLightColorAtPoint(glm::vec3 point) const 
{
	glm::vec3 shadowVector = transform.position - point;
	float distanceToLight = glm::dot(shadowVector, shadowVector);
	return color * intensity * distanceToLight;
}

#endif