#pragma once


#ifndef CAMERA_H
#define CAMERA_H


#include "glm/glm.hpp"
#include "Ray.h"
#include <glm/gtc/matrix_transform.hpp>
// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};



// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{

private:
    // Default camera values
    const float YAW = -90.0f;
    const float PITCH = 0.0f;
    const float SPEED = 2.5f;
    const float SENSITIVITY = 0.1f;
    const float ZOOM = 45.0f;
    glm::vec3 p_e; // Position of the camera
    glm::vec3 p_c; // Position of the center of the screen
    glm::vec3 p_00; // Bottom Left corner of the screen 
    glm::vec3 v_view, v_up; // View and Up vectors that defines the orientation of the camera
    glm::vec3 n_0, n_1; // unit Perpendicular vectors that define the screen
    glm::vec3 n_2; // n2 : v_view but unit vector

    float s_x, s_y; // Dimensions of the screen 
    float d; // Distance of the screen from the camera

public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front = glm::vec3(0, 0, 1);
    glm::vec3 Up = glm::vec3(0, 1, 0);
    glm::vec3 Right = glm::vec3(-1, 0, 0);
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed = 1.0f;
    float MouseSensitivity;
    float Zoom;
    glm::vec3 lower_left_corner;
    glm::vec3 horizontal;
    glm::vec3 vertical;
    glm::vec3 origin;
    Camera(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& up, float sx, float sy, float screen_distance);
    inline Ray GetRay(float u, float v, float width, float height) const;

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            origin += Front * velocity;
        if (direction == BACKWARD)
            origin -= Front * velocity;
        if (direction == LEFT)
            origin -= Right * velocity;
        if (direction == RIGHT)
            origin += Right * velocity;
        if (direction == UP)
            origin += Up * velocity;
        if (direction == DOWN)
            origin -= Up * velocity;
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 135.0f)
            Zoom = 135.0f;
    }

    //Casts a ray and returns a hit
    void ProcessMouseClick(float posX, float posY, int mouseButton)
    {

    }

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up = glm::normalize(glm::cross(Right, Front));
    }
};


Camera::Camera(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& up, float sx, float sy, float screen_distance) : p_e(position), v_view(direction), v_up(up), s_x(sx), s_y(sy), d(screen_distance) {
    lower_left_corner = glm::vec3(-2.0, -1.0, -1.0);
    horizontal = glm::vec3(4.0, 0.0, 0.0); // horizontal range
    vertical = glm::vec3(0.0, 2.0, 0.0);   // vertical range
    origin = glm::vec3(0.0, 0.0, 0.0);
}



Ray Camera::GetRay(float u, float v, float width, float height) const {
    return Ray(origin, lower_left_corner + u/width * horizontal + v/height * vertical - origin);
}


#endif