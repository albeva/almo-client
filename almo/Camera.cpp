//
//  Camera.cpp
//  almo
//
//  Created by Albert Varaksin on 20/08/2017.
//  Copyright © 2017 Albert Varaksin. All rights reserved.
//
#include "Camera.hpp"
#include <glm/gtx/transform.hpp>
using namespace almo;

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
: m_front(glm::vec3(0.0f, 0.0f, -1.0f))
, m_movementSpeed(SPEED)
, m_mouseSensitivity(SENSITIVTY)
, m_zoom(ZOOM)
, m_position(position)
, m_worldUp(up)
, m_yaw(yaw)
, m_pitch(pitch)
{
    this->updateCameraVectors();
}



// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(m_position, m_position + m_front, m_up);
}


void Camera::processKeyboard(Camera::Direction direction, float deltaTime)
{
    float velocity = m_movementSpeed * deltaTime;
    switch (direction) {
        case FORWARD:
            m_position += m_front * velocity;
            break;
        case BACKWARD:
            m_position -= m_front * velocity;
            break;
        case LEFT:
            m_position -= m_right * velocity;
            break;
        case RIGHT:
            m_position += m_right * velocity;
            break;
    }
}


void Camera::processMouseMovement(float xOffset, float yOffset, bool constrainPitch)
{
    xOffset *= m_mouseSensitivity;
    yOffset *= m_mouseSensitivity;

    m_yaw   += xOffset;
    m_pitch += yOffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch) {
        if (m_pitch > 89.0f) {
            m_pitch = 89.0f;
        }

        if (m_pitch < -89.0f) {
            m_pitch = -89.0f;
        }
    }

    // Update Front, Right and Up Vectors using the updated Eular angles
    updateCameraVectors();
}


void Camera::processMouseScroll(float yOffset)
{
    if (m_zoom >= 1.0f && m_zoom <= 45.0f) {
        m_zoom -= yOffset;
    }

    if (m_zoom < 1.0f) {
        m_zoom = 1.0f;
    } else if (m_zoom > 45.0f) {
        m_zoom = 45.0f;
    }
}


// Calculates the front vector from the Camera's (updated) Eular Angles
void Camera::updateCameraVectors( )
{
    // Calculate the new Front vector
    glm::vec3 front;
    front.x = cos( glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin( glm::radians(m_pitch));
    front.z = sin( glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}
