#ifndef RAY_H
#define RAY_H

#include "glm/glm.hpp"

class Ray {
private:
	glm::vec3 p_0; //starting point
	glm::vec3 n; //direction of ray
public:
	Ray();
	Ray(const glm::vec3& point, const glm::vec3& direction);
	~Ray();

	inline glm::vec3 GetPoint(float t) const; //Get a point "t" distance away from the starting point
	inline glm::vec3 GetStartingPoint() const { return p_0; }
	inline glm::vec3 GetDirection() const { return n; }
};

Ray::Ray() {}
Ray::~Ray() {}

Ray::Ray(const glm::vec3& point, const glm::vec3& direction) : p_0(point) {
	n = direction;
	n = glm::normalize(n);
}
inline glm::vec3 Ray::GetPoint(float t) const {
	return p_0 + t * n; // Returns the point calculated using the passed paramter
}
#endif