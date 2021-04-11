#ifndef MATERIALH
#define MATERIALH
#include "glm/glm.hpp"
class Material {
private:
    float m_k_d, m_k_s; // Diffuse and specular coefficients
    float m_refractive_index; // Refractive index of the material
    glm::vec3 m_color; // color of the material
public:
    bool isUnlit;
    bool isReflective;
    bool isRefrative;
    Material();
    ~Material();
    Material(glm::vec3& color, float k_diffuse, float k_specular, float refractive_index);
    virtual glm::vec3 getColor() { return m_color; }
    virtual glm::vec3 getBilinearColor(float u, float v) { return m_color; } // returns bilinear color 

    inline float getKDiffuse() { return m_k_d; }
    inline float getKSpecular() { return m_k_s; }
    inline float getRefractiveIndex() { return m_refractive_index; }
};

Material::Material() {
    m_color = glm::vec3(1.0f, 1.0f, 1.0f);
    m_k_d = 1.0;
    m_k_s = 1.0;
    m_refractive_index = 1.0;
}

Material::~Material() {}

Material::Material(glm::vec3& color, float k_diffuse, float k_specular, float refractive_index)
    : m_color(color), m_k_d(k_diffuse), m_k_s(k_specular), m_refractive_index(refractive_index) {}

#endif