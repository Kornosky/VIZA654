
#ifndef TRIANGULARMESH
#define TRIANGULARMESH

#include "VisibleObject.h"
#include "Plane.h"
#include "Triangle.h"

class TriangularMesh : public VisibleObject {
private:
    glm::vec3 p_0; // Center of the mesh
    vector<glm::vec3> vertices; // vector of vertices, each triangle is assumed to be three consecutive vertices
    vector<Triangle> triangles; // vector of triangles
    vector<glm::vec3>  normals; // vector of normals
    int n_triangles; // Number of triangles

public:
        TriangularMesh();
        TriangularMesh(glm::vec3& center, vector<glm::vec3> vertex_data, vector<glm::vec3>  normal_data, int no_of_triangles);
        ~TriangularMesh();
    // The function will return a glm::vec3 with x : Parameter t, y : slope of hit, z : if hit (+ve if hit, -ve otherwise)
        glm::vec3 getIntersectInfo(const Ray& incoming) const;

    // The normal to the sphere 
        glm::vec3 getNormalAtPoint(glm::vec3& point, int id_triangle) const;

    // The color
      glm::vec3 getColor(glm::vec3& point, int id_triangle) const;

        //int getTypeID() const { return TMESH_TYPE_ID; }
};

    TriangularMesh::TriangularMesh() {}

    TriangularMesh::~TriangularMesh() {}

    TriangularMesh::TriangularMesh(glm::vec3& center, vector<glm::vec3> vertex_data, vector<glm::vec3>  normal_data, int no_of_triangles)
        : p_0(center), vertices(vertex_data), normals(normal_data), n_triangles(no_of_triangles) {
        vector<glm::vec3> pointVector;
        vector<glm::vec3> normalVector;
        
        for (int i = 0; i < no_of_triangles; i++) { //TODO this is awful
            vertices[i] += p_0;
            vertices[i + 1] += p_0;
            vertices[i + 2] += p_0;
            //Add vertices
            pointVector.push_back(glm::vec3(vertices[i].x , vertices[i].y , vertices[i].z));
            pointVector.push_back(glm::vec3(vertices[i+1].x , vertices[i+1].y , vertices[i+1].z));
            pointVector.push_back(glm::vec3(vertices[i+2].x , vertices[i+2].y , vertices[i+2].z));       

            //Add normals
            normalVector.push_back(glm::vec3(normals[i].x , normals[i].y , normals[i].z));
            normalVector.push_back(glm::vec3(normals[i+1].x , normals[i+1].y , normals[i+1].z));
            normalVector.push_back(glm::vec3(normals[i+2].x , normals[i+2].y , normals[i+2].z));
            triangles.push_back(Triangle(pointVector, normalVector));
            normalVector.clear();
            pointVector.clear();
        }
        isPrimative = false;
    }

    glm::vec3 TriangularMesh::getIntersectInfo(const Ray& incoming) const {
    
        glm::vec3 intersection(0.0f, 0.0f, 0.0f);
        float id_triangle = -1;
        float if_intersect = -1;
        float ray_t = 10000;

        glm::vec3 A(0.0f, 0.0f, 0.0f);
        glm::vec3 A_0(0.0f, 0.0f, 0.0f);
        glm::vec3 A_1(0.0f, 0.0f, 0.0f);
        glm::vec3 A_2(0.0f, 0.0f, 0.0f);
        glm::vec3 n(0.0f, 0.0f, 0.0f);
        glm::vec3 plane_intersection(0.0f, 0.0f, 0.0f);
        glm::vec3 p_h(0.0f, 0.0f, 0.0f);

        glm::vec3 p_i, p_ip1, p_im1;
        int i = 0;
        float s, t, mst;
        for each (Triangle tri in triangles) {
            //Check for intersection
            intersection = tri.LocalIntersection(incoming);
            if (intersection[2] > 0) //if intersected then break
            {
                intersection[1] = i;
                return intersection;
            }
            i++;
        }
       return intersection;
}

    glm::vec3 TriangularMesh::getNormalAtPoint(glm::vec3& point, int id_triangle) const {
    int p_i = 3 * id_triangle + 1;
    int p_im1 = 3 * id_triangle;
    int p_ip1 = 3 * id_triangle + 2;

    glm::vec3 A = cross(vertices[p_i] - vertices[p_im1], vertices[p_ip1] - vertices[p_i]) / 2.0f;
    glm::vec3 A_0 = cross(point - vertices[p_ip1], vertices[p_i] - point) / 2.0f;
    glm::vec3 A_1 = cross(point - vertices[p_im1], vertices[p_ip1] - point) / 2.0f;
    glm::vec3 A_2 = cross(point - vertices[p_i], vertices[p_im1] - point) / 2.0f;
    glm::vec3 n = glm::normalize(A);

    float s = dot(n, A_1) / A.length();
    float t = dot(n, A_2) / A.length();
    float mst = dot(n, A_0) / A.length();

    glm::vec3 n_h = mst * normals[p_im1] + s * normals[p_i] + t * normals[p_ip1];
    n_h = glm::normalize(n_h);

    return n_h;
}

  glm::vec3 TriangularMesh::getColor(glm::vec3& point, int id_triangle) const {
    if (mat != NULL) {
        int p_i = 3 * id_triangle + 1;
        int p_im1 = 3 * id_triangle;
        int p_ip1 = 3 * id_triangle + 2;

        glm::vec3 A = cross(vertices[p_i] - vertices[p_im1], vertices[p_ip1] - vertices[p_i]) / 2.0f;
        glm::vec3 A_1 = cross(point - vertices[p_im1], vertices[p_ip1] - point) / 2.0f;
        glm::vec3 A_2 = cross(point - vertices[p_i], vertices[p_im1] - point) / 2.0f;
        glm::vec3 n = glm::normalize(A);

        float s = dot(n, A_1) / A.length();
        float t = dot(n, A_2) / A.length();

        float u = (1 - s - t) * 0.1 + s * 0.9 + t * 0.9;
        float v = (1 - s - t) * 0.1 + s * 0.1 + t * 0.9;
        return mat->getBilinearColor(u, v);
    }
    return glm::vec3(255,0,255);
}


#endif