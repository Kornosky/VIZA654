// =============================================================================
// VIZA654/CSCE646 at Texas A&M University
// Project 0
// Created by Shenyao Ke based from Ariel Chisholm's template
// 09/28/14
// 
// Copyright (c) 2014 Shenyao Ke. All rights reserved.
// =============================================================================

// Current Operations
// Left-Mouse button on display window to get current pixel values
// Mid-Mouse button to save image
// Right-Mouse button to close window
// Press R to re-render the image
#include <cstdlib>
#include <iostream>
#include "GL/glew.h"
#include "GL/glfw3.h"
#include "ImageData.h"
#include "Camera.h"
#include "VisibleObject.h"
#include <Sphere.h>
#include <Plane.h>

using namespace std;

// ==================
// WORLD
// ==================
Camera* mainCamera;
// =============================================================================
// These variables will store the input ppm image's width, height, and color
// =============================================================================
// Define the initial window size
const static int width(1280), height(720);
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
Sphere* mainSphere;
// =============================================================================
// Initialize parameters
// =============================================================================

void InitParameters()
{
	//Create camrea -2000=x
	mainCamera = new Camera(glm::vec3(-20, 0, 0), glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), (width), height, 10);

	//Create sphere
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	float radius = 50;
	Sphere* sphere = new Sphere(position, radius);
	mainSphere = sphere;
	objectsInScene.push_back((VisibleObject*)sphere);
	
	//Create sphere
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	radius = 10;
	Sphere* sphere2 = new Sphere(position, radius);
	objectsInScene.push_back((VisibleObject*)sphere2);
}


bool GetHitObject(Ray incoming_ray, VisibleObject*& closest_object, glm::vec3& extraIntersectInfo) {

	float min_t = 0.0f;
	bool if_t_encountered = false;
	glm::vec3 intersectInfo;

	for (int i = 0; i < objectsInScene.size(); i++) {

		intersectInfo = objectsInScene[i]->getIntersectInfo(incoming_ray);

		if (!if_t_encountered && intersectInfo[2] > 0.0f) {
			if_t_encountered = true;
			min_t = intersectInfo[0];
			extraIntersectInfo = intersectInfo;
			closest_object = objectsInScene[i];
		}
		else if (if_t_encountered && intersectInfo[2] > 0.0f) {
			if (min_t > intersectInfo[0]) {
				min_t = intersectInfo[0];
				extraIntersectInfo = intersectInfo;
				closest_object = objectsInScene[i];
			}
		}

	}

	return if_t_encountered;
}

glm::vec3 RayTrace(Ray incoming_ray, int steps) { //IDK what steps are for
	glm::vec3 final_color(0, 0, 255);

	Ray current_ray = incoming_ray;

	glm::vec3 intersectInfo;

	VisibleObject* closest_object;

	glm::vec3 point_of_intersection, normal, refl_dir, refr_dir, ray_dir;
	float min_t;
	Ray reflected_ray, refracted_ray;

	for (int i = 0; i < steps; i++) {
		bool if_intersected = GetHitObject(current_ray, closest_object, intersectInfo);

		if (if_intersected) {
			min_t = intersectInfo[0]; 
			point_of_intersection = current_ray.GetPoint(min_t);
			final_color = glm::vec3(255, 255, 255); //make white if hit
		}
		else {
			break;
		}
	}
	return final_color;

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
			glm::vec3 final_color = RayTrace(mainCamera->GetRay(i - width / 2, j - height / 2), 1);
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
		mainSphere->MoveSphere(FORWARD, deltaTime);//mainCamera->ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		mainSphere->MoveSphere(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		mainSphere->MoveSphere(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		mainSphere->MoveSphere(RIGHT, deltaTime);
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
