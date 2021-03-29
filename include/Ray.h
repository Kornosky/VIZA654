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
	inline glm::vec2 Ray::RenderRay(float T) const;
};

Ray::Ray() {}
Ray::~Ray() {}

Ray::Ray(const glm::vec3& point, const glm::vec3& direction) : p_0(point), n(glm::normalize(direction)) {}

inline glm::vec3 Ray::GetPoint(float t) const {
	return p_0 + t * n; // Returns the point calculated using the passed paramter
}


inline glm::vec2 Ray::RenderRay(float T) const
{

	//glm::vec3 v_0 = this->GetPoint(100) -  this->p_0;

	//float max = std::max({ v_0.x, v_0.y, v_0.z });

	//p = p_0 + (v_0) / glm::max(glm::length(p_0), glm::length(v_0));
	//float A = a / std::max(| a | , | b | );
	//float B = b / std::max(| a | , | b | );
	//float C = c / std::max(| a | , | b | );


	//float x = x0 + A T

	//float y = y0 + B T

	//float z = y0 + C T

	//int X = (int)(x + 0.5);

	//int Y = (int)(y + 0.5);

	//return glm::vec2(X, Y);
}
#endif