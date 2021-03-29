#ifndef LIGHT_H
#define LIGHT_H

#include "Ray.h"
#include "Transform.h"

class Light {
private:
protected:
	float intensity;
	glm::vec3 color;
public:
	Light();
	~Light();
	Transform transform;

	virtual glm::vec3 GetShadowVector(glm::vec3 point) const = 0 {};
	//virtual glm::vec3 GetIntersectInfo(Ray incoming_ray) const = 0 {};
	virtual float GetAbsorbDistance(Ray incoming_ray) const = 0 {};

	//inline virtual void Flicker(float deltaTime);
};

Light::Light() {}
Light::~Light() {}
//inline void Light::Flicker(float deltaTime) { intensity *= deltaTime * (rand() / RAND_MAX); };

#endif
