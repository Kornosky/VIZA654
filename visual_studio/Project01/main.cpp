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

using namespace std;

// =============================================================================
// These variables will store the input ppm image's width, height, and color
// =============================================================================
// Define the initial window size
static int width(1280), height(720);
// Actual pixel data array
unsigned char *pixmap;
// GLFW display window and window name on the menu bar
GLFWwindow* window = nullptr;
const char* win_name = "VIZA654";
// Output image file name
const string img_name = "VIZA654.png";
// =============================================================================
// Initialize parameters
// =============================================================================
static void initPara()
{
	pixmap = new unsigned char[width * height * 3];
}
// =============================================================================
// Image operation function
// =============================================================================
void setPixels()
{
	// Example to read image, modify and display it
	// Initialize a image instance by reading an image file
	// Check the class file to find more construction methods
	ImageData img("viz.jpg");
	// Change the global variable to make display window fit the image size
	width = img.getWidth();
	height = img.getHeight();
	// Traversal pixels in the image
	for (int j = 0; j < height; j+=10)
	{
		for (int i = 0; i < width; i+=10)
		{
			// Get image color at position i, j
			ColorRGBA cur_color = img.getRGBA(i, j);
			// Change the pixel color at i, j
			// For example, make the color half of the original value
			img.setRGBA(i, j, cur_color * 0.5);
		}
	}
	// Read color values into pixmap array
	img.getPixels(pixmap);
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
	glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, pixmap);

	glFlush();
}
///*
static void processCursor(GLFWwindow* window, double x, double y)
{
	int cx = static_cast<int>(x);
	int cy = height - static_cast<int>(y);
	int idx = 3 * (cx + width * cy);
	cout << "Color at " << cx << ", " << cy << " is " <<
		(int)pixmap[idx] << " " << (int)pixmap[idx + 1] << " " << (int)pixmap[idx + 2] << endl;
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
			ImageData img(width, height, pixmap);
			img.writeFile(img_name);
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
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		setPixels();
	}
	if (key == GLFW_KEY_Q && action == GLFW_PRESS)
		exit(0);
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
	
	initPara();
	setPixels();

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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		windowDisplay();
		glfwSetMouseButtonCallback(window, processMouse);

		glfwSetKeyCallback(window, processKeyboard);
		glfwPollEvents();

		glfwSwapBuffers(window);
	}
	glfwTerminate();

	delete [] pixmap;

	return 0;
}
