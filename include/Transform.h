#include <glm/ext/vector_float3.hpp>
#ifndef TRANSFORM_H
#define TRANSFORM_H

struct Transform {
public:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
//public:
//	Transform();
//	Transform(const glm::vec3& point, const glm::vec3& direction);
//	~Transform();
//
//	inline glm::vec3 MakeSphere();
};

#endif