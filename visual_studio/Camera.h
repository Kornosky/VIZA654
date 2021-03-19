
#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
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
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    Camera(const Vector3& position, const Vector3& direction, const Vector3& up, float sx, float sy, float screen_distance);


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
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
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


Camera::Camera(const Vector3& position, const Vector3& direction, const Vector3& up, float sx, float sy, float screen_distance) : p_e(position), v_view(direction), v_up(up), s_x(sx), s_y(sy), d(screen_distance) {
    n_0 = cross(v_view, v_up);
    n_0.make_unit_vector();

    n_1 = cross(n_0, v_view);
    n_1.make_unit_vector();

    n_2 = unit_vector(v_view);

    p_c = p_e + n_2 * d; // Going to the center of the screen from the camera position in the direction of the view vector

    p_00 = p_c - (s_x / 2.0f) * n_0 - (s_y / 2.0f) * n_1; // The 0,0 for the screen : Bottom Left corner

    p_lens_00 = p_e - (s_lens_x / 2.0f) * n_0 - (s_lens_y / 2.0f) * n_1;
    s_lens_x = s_x; // Setting the lens size
    s_lens_y = s_y;

    cam_focus = 5.0f;

}





#endif