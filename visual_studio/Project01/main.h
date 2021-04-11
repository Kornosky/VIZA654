void ShadowCalculation(glm::vec3& intersectInfo, Ray& shadowRay, glm::vec3& lightPosition, glm::vec3& point_of_intersection, float& shadow_intensity);

void ComputeColor(VisibleObject* closest_object, glm::vec3& point_of_intersection, glm::vec3& intersectInfo, glm::vec3& reflection, glm::vec3& eye_direction, glm::vec3& final_color);
