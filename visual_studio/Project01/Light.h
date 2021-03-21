#ifndef LIGHT_H
#define LIGHT_H

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

	virtual glm::vec3 GetLightColorAtPoint(glm::vec3 point) const = 0 {};

	//inline virtual void Flicker(float deltaTime);
};

Light::Light() {}
Light::~Light() {}
//inline void Light::Flicker(float deltaTime) { intensity *= deltaTime * (rand() / RAND_MAX); };

#endif
