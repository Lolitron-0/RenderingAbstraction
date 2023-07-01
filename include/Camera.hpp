#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Ra
{
    enum class CameraDirection
    {
        Forward,
        Backward,
        Right,
        Left,
        Up,
        Down
    };

    /// Camera abstraction class
    class Camera
    {
    public:
        Camera(const glm::vec3& position, float aspectRatio, const glm::vec3& worldUp = { 0.f, 1.f, 0.f });

        virtual ~Camera() = default;

        glm::vec3 GetPosition() const { return m_Position; }
        glm::vec2 GetRotation() const { return { m_Yaw, m_Pitch }; }
        float GetYaw() const { return m_Yaw; }
        float GetPitch() const { return m_Pitch; }

        /// Returns camera front vector
        glm::vec3 GetLookDirection() const { return m_Front; }

        /// Returns current view-projection matrix
        glm::mat4 GetViewProjection() const { return m_ViewProjMatrix; }

        void SetPosition(const glm::vec3& pos) { m_Position = pos; RecalculateMatrix_(); }
        void SetPitch(float pitch) { m_Pitch = pitch; RecalculateMatrix_(); }
        void SetYaw(float yaw) { m_Yaw = yaw; RecalculateMatrix_(); }
        /// Sets camera rotation with one vector ( yaw ; pitch )
        void SetRotation(const glm::vec2& yawPitch) { m_Yaw = yawPitch.x; m_Pitch = yawPitch.y; RecalculateMatrix_(); }

        /// Adds val to camera's pitch rotation (in degrees)
        void ShiftPitch(float val) { m_Pitch += val; RecalculateMatrix_(); }
        /// Adds val to camera's yaw rotation (in degrees)
        void ShiftYaw(float val) { m_Yaw += val; RecalculateMatrix_(); }

        /// Adds some vector to camera current position
        void ShiftPosition(const glm::vec3& val) { m_Position += val; RecalculateMatrix_(); }
        /// Moves camera in chosen direction on given value
        void ShiftPosition(const CameraDirection& dir, float val);

    protected:
        void RecalculateMatrix_();

        glm::vec3 m_Position;
        float m_Yaw;
        float m_Pitch;
        glm::vec3 m_Rotation;

        glm::vec3 m_WorldUp;

        glm::vec3 m_Front;
        glm::vec3 m_Up;
        glm::vec3 m_Right;

        glm::mat4 m_ProjMatrix;
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ViewProjMatrix;
    };
}