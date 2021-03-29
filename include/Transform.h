#include <glm/ext/vector_float3.hpp>
#ifndef TRANSFORM_H
#define TRANSFORM_H

struct Transform {
public:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
public:
	Transform();
	Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
	~Transform();

	inline glm::vec3 MakeSphere();
};

inline Transform::Transform()
{
}

Transform::Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
{
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;
}
inline Transform::~Transform()
{
}
#endif