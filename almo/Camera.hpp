//
//  Camera.hpp
//  almo
//
//  Created by Albert Varaksin on 20/08/2017.
//  Copyright © 2017 Albert Varaksin. All rights reserved.
//
#pragma once
#include <glm/glm.hpp>

namespace almo {

    // An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
    class Camera
    {
    public:
        // Default camera values
        static constexpr float YAW        = -90.0f;
        static constexpr float PITCH      =  0.0f;
        static constexpr float SPEED      =  6.0f;
        static constexpr float SENSITIVTY =  0.25f;
        static constexpr float ZOOM       =  45.0f;

        enum Direction {
            FORWARD, BACKWARD, LEFT, RIGHT
        };

        // Constructor with vectors
        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = Camera::YAW, float pitch = Camera::PITCH);

        // Returns the view matrix calculated using Eular Angles and the LookAt Matrix
        glm::mat4 getViewMatrix();

        // Processes input received from any keyboard-like input system.
        // Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
        void processKeyboard(Camera::Direction direction, float deltaTime);

        // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
        void processMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);

        // Processes input received from a mouse scroll-wheel event.
        // Only requires input on the vertical wheel-axis
        void processMouseScroll(float yOffset);

        float getZoom( )
        {
            return this->m_zoom;
        }

    private:
        // Camera Attributes
        glm::vec3 m_position;
        glm::vec3 m_front;
        glm::vec3 m_up;
        glm::vec3 m_right;
        glm::vec3 m_worldUp;

        // Eular Angles
        float m_yaw;
        float m_pitch;

        // Camera options
        float m_movementSpeed;
        float m_mouseSensitivity;
        float m_zoom;

        // Calculates the front vector from the Camera's (updated) Eular Angles
        void updateCameraVectors();
    };

}
