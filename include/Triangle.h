#define TRIANGLE_H
#ifndef TRIGANGLE_H
#include "glm/glm.hpp"
#include "Ray.h"
#include <cstdlib>
class Triangle
{
private:

public:
	glm::vec3 points[3];
	glm::vec3 edges[2];
	glm::vec3 normal;
	vector<glm::vec3> vertNormals;
	Triangle() {};
	
	Triangle(vector<glm::vec3> incoming_points, vector<glm::vec3> vN) {
		vertNormals = vN;
		memcpy(points, incoming_points.data(), sizeof(points));
		edges[0] = incoming_points[1] - incoming_points[0];
		edges[1] = incoming_points[2] - incoming_points[0];
		normal = glm::normalize(glm::cross(edges[0], edges[1]));
	}
	~Triangle() {};

	glm::vec3 LocalIntersection(const Ray& ray)
	{
		glm::vec3 intersectionInfo(0.0f, 0.0f, 0.0f);
		glm::vec3 dir_cross_e2 = glm::cross(ray.GetDirection(), edges[1]);
		float determinant = glm::dot(edges[0], dir_cross_e2);
		//Check if ray is parallel to triange
		if (glm::abs(determinant) > .001f)
		{
			float f = 1.0 / determinant;
			glm::vec3 p1_to_origin = ray.GetStartingPoint() - points[0];
			float u = f * glm::dot(p1_to_origin, dir_cross_e2);
			//Check if hit point lands within u barycentric coordinate
			if (u > 0 && u < 1)
			{
				glm::vec3 origin_cross_e1 = glm::cross(p1_to_origin, edges[0]);
				float v = f * glm::dot(ray.GetDirection(), origin_cross_e1);
				//Check if hit point lands within v barycentric coordinate
				if (v > 0 && (u + v) < 1)
				{
					//Get distance
					float t = f * glm::dot(edges[1], origin_cross_e1);
					return glm::vec3(t, 1.0f, 1.0f);
				}
			}
		}

		return glm::vec3(0.0f, 0.0f, 0.0f);
	}
};

#endif // !TRIGANGLE_H

