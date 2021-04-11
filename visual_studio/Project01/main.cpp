#define STB_IMAGE_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION 
#include "stb_image.h"
#include "tiny_obj_loader.h"

#include <cstdlib>
#include <iostream>
#include "GL/glew.h"
#include "GL/glfw3.h"
#include "ImageData.h"
#include "Camera.h"
#include "VisibleObject.h"
#include <Sphere.h>
#include <Plane.h>
#include "Light.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "SkySphere.h"
#include "SpotLight.h"
#include "AreaLight.h"
#include "Plane.h"
#include "ColorHelpers.h"
#include "TextureMaterial.h"
#include "TriangularMesh.h"
#include "main.h"
/***

Tasks:
- !Texture mapping
- Importing geometry
- Triangles!
- Sky box looks like ass and causes a crash
- different lights
- improve redner times
- """"" next assignment """"
- Then do specular/reflection/refraction/fresnel
Interested in adding: 
- BVH's 

**/
using namespace std;
// ==================
// WORLD
// ==================
Camera* mainCamera;
// =============================================================================
// These variables will store the input ppm image's width, height, and color
// =============================================================================
// Define the initial window 
const static int width(512), height(288);
// Actual pixel data array
GLubyte pixmap[height][width][3];
// GLFW display window and window name on the menu bar
GLFWwindow* window = nullptr;
const char* win_name = "VIZA654";
// Output image file name
const string img_name = "VIZA654.png";
//TIMING
float deltaTime = 0;
float lastFrame = 0;
//WORLD =================
std::vector<VisibleObject*> objectsInScene;
std::vector<VisibleObject*> gizmosInScene;
std::vector<Light*> lightsInScene;
std::vector<Ray*> raysToDraw;


// vertex_data and normal_data will be appropriately initialized
// Returns number of triangles
int loadOBJ(std::string  file_name, std::vector<glm::vec3>& vertex_data, std::vector<glm::vec3>& normal_data) {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn;
	std::string err;

	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, file_name.c_str()); // Will return triangles by default

	if (!warn.empty()) {
		std::cout << warn << std::endl;
	}
	if (!err.empty()) {
		std::cerr << err << std::endl;
	}
	if (!ret) {
		std::cerr << "It failed!" << std::endl;
	}

	int total_traingles_verts = 0;

	for (size_t s = 0; s < shapes.size(); s++) {
		// Loop over faces(polygon)
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			int fv = shapes[s].mesh.num_face_vertices[f];
			total_traingles_verts += fv;
		}
	}	

	for (const auto& shape : shapes) {
		// shape name
		std::cout << shape.name << std::endl;
		size_t index_offset = 0;// indent offset
		for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
			// number of vertices composing face f
			int num_vertices = shape.mesh.num_face_vertices[f];
			for (size_t v = 0; v < num_vertices; v++) {
				// access to vertex
				tinyobj::index_t idx = shape.mesh.indices[index_offset + v];
				// vertex coordinates
				float vx = attrib.vertices[3 * idx.vertex_index + 0];
				float vy = attrib.vertices[3 * idx.vertex_index + 1];
				float vz = attrib.vertices[3 * idx.vertex_index + 2];
				vertex_data.push_back(glm::vec3(vx, vy, vz));
				// vertex normal
				float nx = attrib.normals[3 * idx.normal_index + 0];
				float ny = attrib.normals[3 * idx.normal_index + 1];
				float nz = attrib.normals[3 * idx.normal_index + 2];
				normal_data.push_back(glm::vec3(nx, ny, nz));
				// vertex texture coordinates
		/*		tinyobj::real_t tx =
					attrib.texcoords[2 * idx.texcoord_index + 0];
				tinyobj::real_t ty =
					attrib.texcoords[2 * idx.texcoord_index + 1];*/
				// vertex color
				float red = attrib.colors[3 * idx.vertex_index + 0];
				float green = attrib.colors[3 * idx.vertex_index + 1];
				float blue = attrib.colors[3 * idx.vertex_index + 2];
			}
			index_offset += num_vertices;
			// per-face material
			shape.mesh.material_ids[f];
		}
	}

	return int(total_traingles_verts / 3);
}

void InitParameters()
{
	// Loading images for textures
	int const n_imgs =1 ;
	unsigned char* host_imgs[n_imgs];
	int img_w[n_imgs], img_h[n_imgs], img_chns[n_imgs];

	// -------- Load Images Here ------- // TODO not have such an ass way of putting in images
	host_imgs[0] = stbi_load("E:/ImageProcessing/ProjectFiles/ImageSynthesisRayCaster/VIZA654/textures/Planet.png", &img_w[0], &img_h[0], &img_chns[0], 0);
	if (stbi_failure_reason())
		std::cout << stbi_failure_reason();

	//Create camrea 
	mainCamera = new Camera(glm::vec3(0, .5f, 10.0f), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0), width, height, 1.0f);
	glm::vec3 position = glm::vec3(0.0f, 1.0f, -2.0f);

	// Read our .obj file
	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals; // Won't be used at the moment.

	// Loading Meshes and Normals
	std::string obj_file_name = "E:/ImageProcessing/ProjectFiles/ImageSynthesisRayCaster/VIZA654/models/cube.obj";
	int no_of_triangles = loadOBJ(obj_file_name, vertices, normals);
	//vertices.clear();
	//vertices.push_back(glm::vec3(0.0f, 1.0f, -5.0f));
	//vertices.push_back(glm::vec3(1.0f, 1.0f, -6.0f));
	//vertices.push_back(glm::vec3(0.0f, 1.5f, -5.0f));
	TriangularMesh* t_mesh = new TriangularMesh(position, vertices, normals, no_of_triangles);
	//objectsInScene.push_back(t_mesh);

	//Create sphere
	position = glm::vec3(0.0f, 1.0f, -12.0f);
	float radius = 2.0f;
	Sphere* sphere = new Sphere(position, radius);
	objectsInScene.push_back((VisibleObject*)sphere);
	sphere->color = glm::vec3(255, 0, 0);
	TextureMaterial* mat0 = new TextureMaterial();
	mat0->isUnlit = false;
	mat0->isReflective= true;
	mat0->setColorImage(img_w[0], img_h[0], img_chns[0], host_imgs[0]);
	sphere->setMaterial(*mat0);
	
	position = glm::vec3(1.0f, 1.0f, 4.0f);
	radius = .1f;
	Sphere* sphere4 = new Sphere(position, radius);
	objectsInScene.push_back((VisibleObject*)sphere4);
	sphere4->color = glm::vec3(255, 35, 25);

	//Sky Sphere
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	radius = 2000.0f;
	SkySphere* sphere6 = new SkySphere(position, radius);
	//objectsInScene.push_back((VisibleObject*)sphere6);
	TextureMaterial* mat1 = new TextureMaterial();
	mat1->setColorImage(img_w[0], img_h[0], img_chns[0], host_imgs[0]);
	mat1->isUnlit = true;
	sphere6->setMaterial(*mat1);

	//Create plane
	glm::vec3 point = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 normal = glm::vec3(0.0f, 1, 0.0f);
	Plane* plane = new Plane(point, normal);
	//objectsInScene.push_back((VisibleObject*)plane);
	plane->color = glm::vec3(0, 255, 0);
	//plane->setMaterial(*mat0);

	//Create sphere
	position = glm::vec3(0.0f, 2, 4.0f);
	radius =.7f;
	Sphere* sphere2 = new Sphere(position, radius);
	//objectsInScene.push_back((VisibleObject*)sphere2);
	sphere2->color = glm::vec3(36, 230, 100);

	////Create sphere
	//position = glm::vec3(0.0f, 2, -6.0f);
	//radius = 2.0f;
	//Sphere* sphere3 = new Sphere(position, radius);
	//objectsInScene.push_back((VisibleObject*)sphere3);
	//sphere3->color = glm::vec3(234, 0, 200);

	//Create light
	//position = glm::vec3(0.0f, 3.0f, -4.0f);
	position = glm::vec3(1.0f, 1.3f, -2.0f);
	float intensity = .3f;
	glm::vec3 color(255, 255, 255);
	PointLight* pointLight = new PointLight(position, color, intensity);
	//lightsInScene.push_back((Light*)pointLight);	
	

	position = glm::vec3(-1.0f, 1.3f, -2.0f);
	intensity = .3f;
	//glm::vec3 color(255, 255, 255);
	PointLight* pointLight1 = new PointLight(position, color, intensity);
	//lightsInScene.push_back((Light*)pointLight1);	
	
	radius = .1f;
	Sphere* sphere5 = new Sphere(position, radius);
	//gizmosInScene.push_back((VisibleObject*)sphere5);
	sphere5->color = glm::vec3(255, 255, 25);

	//Create light ---DIRECTIONAL
	position = glm::vec3(1.0f, 1.0f, -5.0f);
	intensity = .3f;
	//glm::vec3 color(255, 255, 255);
	Transform* transform = new Transform(glm::vec3(1, 0, 0), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0));
	DirectionalLight* pointLight2 = new DirectionalLight(*transform, color, intensity);
	lightsInScene.push_back((Light*)pointLight2);

	//Create light -------------------------SPOT
	//intensity = .3f;
	////glm::vec3 color(255, 255, 255);
	//transform = new Transform(glm::vec3(1.0f, 2.0f, -4.0f), glm::vec3(0.3f, -1, 0.02f), glm::vec3(0, 0, 0));
	//Spotlight* spotlight = new Spotlight(*transform, color, intensity);
	//spotlight->falloff_angle = 40.0f;
	//spotlight->beam_angle = 180.0f;
	////lightsInScene.push_back((Light*)spotlight);
	//radius = .1f;
	//Sphere* sphere6 = new Sphere(transform->position, radius);
	////gizmosInScene.push_back((VisibleObject*)sphere6);
	//sphere6->color = glm::vec3(255, 255, 25);
	
	//Create light -------------------------Area
	intensity = .3f;
	//glm::vec3 color(255, 255, 255);
	/*transform = new Transform(glm::vec3(1.0f, 2.0f, -4.0f), glm::vec3(0.3f, -1, 0.02f), glm::vec3(0, 0, 0));
	AreaLight* areaLight = new AreaLight(transform->position, transform->rotation, glm::vec3(0,1,0), 100, 100);
	lightsInScene.push_back((Light*)areaLight);
	radius = .1f;
	Sphere* sphere7 = new Sphere(transform->position, radius);
	gizmosInScene.push_back((VisibleObject*)sphere6);
	sphere7->color = glm::vec3(255, 255, 25);
	gizmosInScene.push_back(sphere7);*/
}
inline bool ifRayIntersected(const glm::vec3& intersectInfo) { return (intersectInfo[2] > 0.0f); }

void DrawRay(Ray& ray)
{
	/*if(raysToDraw.size() < 5)
		raysToDraw.push_back(&ray);*/
}
void DrawRays(Ray incoming_ray, glm::vec3& final_color)
{
	//for (int i = 0; i < raysToDraw.size(); i++) {
	//	if (incoming_ray.GetIntersectionInfo(*raysToDraw[i]))
	//		final_color += glm::vec3(255, 255, 255);
	//}

}

void DrawGizmos(Ray incoming_ray, glm::vec3& final_color) {

	for (int i = 0; i < objectsInScene.size(); i++) //if object is infront, don't draw
	{
		if (objectsInScene[i]->getIntersectInfo(incoming_ray)[2] > 0.0f)
		{
			return;
		}

	}
	for (int i = 0; i < gizmosInScene.size(); i++) {
		if (gizmosInScene[i]->getIntersectInfo(incoming_ray)[2] > 0.0f) {
			final_color = gizmosInScene[i]->color;
		}
	}

}
void DrawLights(Ray incoming_ray, glm::vec3& final_color) {

	//https://blog.demofox.org/2017/01/09/raytracing-reflection-refraction-fresnel-total-internal-reflection-and-beers-law/
	for (int i = 0; i < lightsInScene.size(); i++) { //TODO to implement fake volumetrics using Beer's law
		
		if (lightsInScene[i]->GetAbsorbDistance(incoming_ray))
		{
			final_color = glm::vec3(0, 0, 0);
		}
	/*		glm::vec3 absorb = exp(-glm::vec3(8.0, 2.0, 0.1) * lightsInScene[i]->GetAbsorbDistance(incoming_ray));
			final_color *= absorb;*/
		
	}
}

bool GetHitObject(Ray incoming_ray, VisibleObject*& closest_object, glm::vec3& extraIntersectInfo) {

	float min_t = 0.0f;
	bool if_t_encountered = false;
	glm::vec3 intersectInfo;

	for (int i = 0; i < objectsInScene.size(); i++) {

		intersectInfo = objectsInScene[i]->getIntersectInfo(incoming_ray);

		if (!if_t_encountered && intersectInfo[2] > 0.0f) {
			if_t_encountered = true;
			min_t = glm::abs(intersectInfo[0]);
			extraIntersectInfo = intersectInfo;
			closest_object = objectsInScene[i];
		}
		else if (if_t_encountered && intersectInfo[2] > 0.0f) {
			if (min_t > glm::abs(intersectInfo[0])) {
				min_t = intersectInfo[0];
				extraIntersectInfo = intersectInfo;
				closest_object = objectsInScene[i];
			}
		}

	}

	return if_t_encountered;
}

void ShadowCalculation(Ray& shadowRay, glm::vec3& lightPosition, glm::vec3& point_of_intersection, float& shadow_intensity)
{
	glm::vec3 intersectInfo;
	for (int i = 0; i < objectsInScene.size(); i++) {

		intersectInfo = objectsInScene[i]->getIntersectInfo(shadowRay);

		if (intersectInfo[2] > 0) //greater than 0 means it intersected
		{
			float distance_intersection_light = glm::length(lightPosition - point_of_intersection);
			float t_from_object = intersectInfo[0];

			if (t_from_object < distance_intersection_light && t_from_object >  0.001) //fixes banding
			{
				////shadow_intensity += (distance_intersection_light - distance_from_light -distance_from_object) / distance_intersection_light; Not sure why or how to implement this
				shadow_intensity = 1;
			}
		}
	}
}
glm::vec3 computeColor(VisibleObject* closest_object, Ray& eye_ray, glm::vec3& point_of_intersection, glm::vec3& extraIntersectInfo) {

	int total_lights = lightsInScene.size();

	glm::vec3 normal = closest_object->getNormalAtPoint(point_of_intersection);
	if (extraIntersectInfo[2] > 0.0f) {
		normal *= -1;
	}

	// Special case for Triangular Mesh    
	if (closest_object->getTypeID() == VisibleObject::MESH) {
		//normal = closest_object->getNormalAtPoint(point_of_intersection, getTriangleIDFromIntersectInfo(extraIntersectInfo));
	}
	normal = glm::normalize(normal);

	glm::vec3 object_color = closest_object->getColor(point_of_intersection);

	// Special casee for Triangular Mesh
	if (closest_object->getTypeID() == VisibleObject::MESH) {
		//object_color = closest_object->getColor(point_of_intersection, getTriangleIDFromIntersectInfo(extraIntersectInfo));
	}

	glm::vec3 final_object_color(0.0, 0.0, 0.0);
	glm::vec3 specular_light_color(1.0, 1.0, 1.0);

	glm::vec3 eye = -1.0f * (point_of_intersection - eye_ray.GetStartingPoint());
	eye = glm::normalize(eye);

	glm::vec3 reflection;

	//Iterating through each light
	for (auto& light : lightsInScene) {
		/*	if ((world->getLight(i))->ifSamplingRequired()) {
				(world->getLight(i))->setRandomSamplePosition(rand_state);
			}*/
		glm::vec3 light_direction = light->GetShadowVector(point_of_intersection);
		light_direction = -light_direction;

		// Computing Diffuse 
		float diffuse_intensity = max(0.0f, dot(normal, light_direction));
		// diffuse_intensity = smoothstep(sharp_edge0, sharp_edge1, diffuse_intensity);

		// Computing Specular
		glm::vec3 light_direction_unit = glm::normalize(light_direction);
		reflection = -light_direction + 2.0f * dot(light_direction_unit, normal) * normal;
		reflection = glm::normalize(reflection);
		float specular_intensity = max(0.0f, dot(eye, reflection));
		specular_intensity = pow(specular_intensity, 4);

		// specular_intensity = smoothstep(sharp_edge0, sharp_edge1, specular_intensity);
		specular_intensity = glm::smoothstep(0.8f, 1.0f, specular_intensity);

		float shadow_intensity = 0.0f;
		if (dot(normal, light_direction) >= 0.0f) { // Compute the shadow only if dot(N,L) > 0
			glm::vec3 shadowDirection = light->GetShadowVector(point_of_intersection);
			Ray shadowRay = Ray(point_of_intersection, glm::normalize(shadowDirection));
			ShadowCalculation(shadowRay, light->transform.position, point_of_intersection, shadow_intensity);
			if (shadow_intensity > 1.0f) {
				shadow_intensity = 1.0;
			}
		}

		shadow_intensity = glm::smoothstep(0.0f, 0.2f, shadow_intensity);

		diffuse_intensity = (1.0f - shadow_intensity) * diffuse_intensity;
		specular_intensity = (1.0f - shadow_intensity) * specular_intensity;

		// Computing the final color
		glm::vec3 color_from_light;
		color_from_light = diffuse_intensity * object_color;
		color_from_light = specular_intensity * specular_light_color + (1.0f - specular_intensity) * color_from_light;

		// Adding the color to the final color
		final_object_color += color_from_light;

	}

	if (total_lights > 0) {
		//final_object_color = final_object_color / total_lights;
	}

	// Computing border parameter

	//float border_intensity = 0;
	//if (if_border) {
	//	border_intensity = max(0.0f, 1.0f - abs(dot(normal, eye)));
	//	border_intensity = smoothstep(border_thinkness, border_thinkness + 0.1, border_intensity);
	//}
	//else {
	//	border_intensity = 0;
	//}
//	final_object_color = (1.0f - border_intensity) * final_object_color;

	// Adding Ambient intensity
	//final_object_color = ambient_intensity * object_color + (1 - ambient_intensity) * final_object_color;

	return final_object_color;
}


glm::vec3 RayTrace(Ray incoming_ray, int steps) { //IDK what steps are for
	glm::vec3 finalColor(255, 0, 255);
	

	Ray current_ray = incoming_ray;

	glm::vec3 intersectInfo;

	VisibleObject* closest_object;

	glm::vec3 point_of_intersection, normal, refl_dir, refr_dir, ray_dir, reflection;
	Ray reflected_ray, refracted_ray;

	glm::vec3 eye_direction = (incoming_ray.GetStartingPoint() - point_of_intersection); //eye vector coming from pixel
	eye_direction = glm::normalize(eye_direction);

	for (int i = 0; i < steps; i++) {
		bool if_intersected = GetHitObject(current_ray, closest_object, intersectInfo);
		//If unlit then we don't need to worry about light further light calculations
		if (if_intersected && closest_object && closest_object->mat != NULL && closest_object->mat->isUnlit) //TODO This is awful
		{
			point_of_intersection = current_ray.GetPoint(intersectInfo[0]);
			return finalColor = closest_object->getColor(point_of_intersection);
		}

		if (if_intersected) {
			point_of_intersection = current_ray.GetPoint(intersectInfo[0]);
			finalColor += computeColor(closest_object, Ray(mainCamera->p_e, eye_direction),point_of_intersection, intersectInfo);
		}
		else {
			// draw background, here we are drawing a gradient
			glm::vec3 unit_direction = glm::normalize(incoming_ray.GetDirection());
			float t = 0.5f * (unit_direction.y + 1.0f); // -1~1 --> 0~1
			float j = (1.0f - t);
			glm::vec3 bottomColor(150.0f, 233.0f, 100.0f);
			glm::vec3 topColor(255, 255, 255);
			finalColor = j * bottomColor + t * topColor; // lerp
			break;
		}
	}

	//DrawLights(incoming_ray, final_color);
	DrawGizmos(incoming_ray, finalColor);
	return finalColor;

}

void ComputeColor(VisibleObject* closest_object, glm::vec3& point_of_intersection, glm::vec3& intersectInfo, glm::vec3& reflection, glm::vec3& eye_direction, glm::vec3& final_color)
{
	glm::vec3 specular_light_color(255, 255, 255);
	//Set default color
	//if(closest_object.m)
	glm::vec3 objectColor;
	// Special casee for Triangular Mesh
	if (closest_object->isPrimative) {
		objectColor = closest_object->getColor(point_of_intersection);
	}
	else
	{
		objectColor = closest_object->getColor(point_of_intersection, intersectInfo[1]);

	}
	float shadow_intensity = 0.0f;
	float specular_intensity = 0.0f;
	float diffuse_intensity = 0.0f;

	//Init variables per light
	glm::vec3 normal = closest_object->getNormalAtPoint(point_of_intersection);
	// Special case for Triangular Mesh    
	if (!closest_object->isPrimative) {
		normal = closest_object->getNormalAtPoint(point_of_intersection, intersectInfo[1]);
	}

	//Compute lights by looping through all lights
	for (auto& light : lightsInScene) {
		if (light->hasSamples)
		{
			light->setRandomSamplePosition();
		}

		glm::vec3 shadowDirection = light->GetShadowVector(point_of_intersection);
		Ray shadowRay = Ray(point_of_intersection, glm::normalize(shadowDirection));
		glm::vec3 lightDirection = -shadowDirection;
		glm::vec3 lightPosition = light->transform.position;


		// ---------------- SPECULAR CALCULATIONS -------------------
		reflection = -shadowDirection + 2.0f * dot(shadowDirection, normal) * normal;
		reflection = glm::normalize(reflection);
		specular_intensity += std::max(0.0f, glm::dot(eye_direction, reflection));
		specular_intensity = glm::smoothstep(0.8f, 1.0f, specular_intensity);

		// ---------------- DIFFUSE CALCULATIONS -------------------
		diffuse_intensity += glm::max(0.0f, glm::dot(normal, shadowDirection));
		diffuse_intensity = glm::clamp(0.0f, 1.0f, diffuse_intensity);
		// ---------------- SHADOW CALCULATIONS ------------------- //TODO shadow bias?
		ShadowCalculation(shadowRay, lightPosition, point_of_intersection, shadow_intensity);

	}
	// ---------------- REFLECTION CALCULATIONS -------------------


	//----------------- COMPUTE FINAL PIXEL COLOR -----------------
	specular_intensity = (1.0f - shadow_intensity) * specular_intensity; //TODO fix the specular not being blurred : Also when to use smoothstep?
																		 //glm::vec3 shadow_color = ColorHelpers::GetComplementaryColor(objectColor) * .5f;
	glm::vec3 shadow_color = glm::vec3(0, 0, 0);
	//shadow_intensity = glm::smoothstep(0.0f, 1.0f, shadow_intensity);
	final_color = (shadow_intensity)*shadow_color + objectColor * (diffuse_intensity)+shadow_color * (1 - diffuse_intensity);
	final_color = glm::mix(final_color, specular_light_color, specular_intensity);
}

// =============================================================================
// Image operation function
// =============================================================================
void Render()
{	
	// Traversal pixels in the image
	#pragma omp parallel for
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			glm::vec3 final_color = RayTrace(mainCamera->GetRay(i , j, width, height ), 2);

			//lights
			pixmap[j][i][0] = (GLubyte)final_color.r;
			pixmap[j][i][1] = (GLubyte)final_color.g;
			pixmap[j][i][2] = (GLubyte)final_color.b;
		}
	}
	glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, pixmap);
}

// =============================================================================
// OpenGL Display and Mouse Processing Functions.
// =============================================================================
static void windowResize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, (w / 2), 0, (h / 2), 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
static void windowDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glRasterPos2i(-1, -1);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);


}
///*
static void processCursor(GLFWwindow* window, double x, double y)
{
	int cx = static_cast<int>(x);
	int cy = height - static_cast<int>(y);
	int idx = 3 * (cx + width * cy);
	
}
static void processMouse(GLFWwindow *window, int button, int action, int mods)
{
	switch (button){
	case GLFW_MOUSE_BUTTON_LEFT:
		if (action == GLFW_PRESS)
			glfwSetCursorPosCallback(window, processCursor);
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		if (action == GLFW_PRESS)
		{
		//	ImageData img(width, height, pixmap);
		//	img.writeFile(img_name);
		}
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		if (action == GLFW_PRESS)
			exit(0);
		break;
	}
}
static void processKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		mainCamera->ProcessKeyboard(FORWARD, deltaTime);//mainCamera->ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		mainCamera->ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		mainCamera->ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		mainCamera->ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		mainCamera->ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		mainCamera->ProcessKeyboard(DOWN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
		mainCamera->ProcessMouseScroll(5);
	if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
		mainCamera->ProcessMouseScroll(-5);
}
static void init(void)
{
	glClearColor(1, 1, 1, 1); // Set backgrPlane color.
}


// =============================================================================
// main() Program Entry
// =============================================================================
int main(int argc, char *argv[])
{
	
	InitParameters();
	if (!glfwInit())
	{
		return 1;
	}

	window = glfwCreateWindow(width, height, win_name, NULL, NULL);
	if (!window)
	{
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(window);
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		//processInput(window);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		windowDisplay();
		glfwSetMouseButtonCallback(window, processMouse);
		
		Render();
		glfwSetKeyCallback(window, processKeyboard);
		glfwPollEvents();

		glfwSwapBuffers(window);
	}
	glfwTerminate();

	delete [] pixmap;

	return 0;
}
