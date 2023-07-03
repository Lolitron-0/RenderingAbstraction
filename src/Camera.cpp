#include "rapch.h"
#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Ra
{
    Ra::Camera::Camera(const glm::vec3& position, float aspectRatio, const glm::vec3& worldUp)
        : m_Position(position), m_WorldUp(glm::normalize(worldUp)), m_Yaw(-90), m_Pitch(0), m_ViewMatrix(1.f), m_ProjMatrix(1.f)
    {
        m_ProjMatrix = glm::mat4(1.f);
        m_ProjMatrix = glm::perspective(glm::radians(45.f), aspectRatio, .1f, 100.f);

        RecalculateMatrix_();
    }

    void Camera::ShiftPosition(const CameraDirection& dir, float val)
    {
        switch (dir)
        {
        case CameraDirection::Forward:
            m_Position += m_Front * val;
            break;
        case CameraDirection::Backward:
            m_Position -= m_Front * val;
            break;
        case CameraDirection::Right:
            m_Position += m_Right * val;
            break;
        case CameraDirection::Left:
            m_Position -= m_Right * val;
            break;
        case CameraDirection::Up:
            m_Position += m_Up * val;
            break;
        case CameraDirection::Down:
            m_Position -= m_Up * val;
            break;
        }

        RecalculateMatrix_();
    }

    void Camera::SetPerspective(float fov, float aspectRatio, float near, float far)
    {
        m_ProjMatrix = glm::mat4(1.f);
        m_ProjMatrix = glm::perspective(glm::radians(fov), aspectRatio, near, far);

        m_ViewProjMatrix = m_ProjMatrix * m_ViewMatrix;
    }

    void Camera::RecalculateMatrix_()
    {
        glm::vec3 front;
        front.x = glm::cos(glm::radians(m_Yaw)) * glm::cos(glm::radians(m_Pitch));
        front.y = glm::sin(glm::radians(m_Pitch));
        front.z = glm::sin(glm::radians(m_Yaw)) * glm::cos(glm::radians(m_Pitch));
        m_Front = glm::normalize(front);

        m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
        m_Up = glm::normalize(glm::cross(m_Right, m_Front));

        m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
        m_ViewProjMatrix = m_ProjMatrix * m_ViewMatrix;
    }

}