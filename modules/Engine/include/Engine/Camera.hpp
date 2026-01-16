///
/// @file Camera.hpp
/// @brief This file contains the camera class declaration
/// @namespace cae
///

#pragma once

#include "Engine/Common.hpp"

#include <glm/glm.hpp>

namespace cae
{

    ///
    /// @class Camera
    /// @brief Class for camera
    /// @namespace cae
    ///
    class Camera
    {
        public:
            Camera() = default;
            ~Camera() = default;

            Camera(const Camera &) = delete;
            Camera &operator=(const Camera &) = delete;
            Camera(Camera &&) = delete;
            Camera &operator=(Camera &&) = delete;

            void setName(const std::string &name) { m_name = name; }
            void setPosition(const glm::vec3 &position) { m_position = position; }
            void setRotation(const glm::vec3 &rotation) { m_rotation = rotation; }
            void setDirection(const glm::vec3 &direction) { m_direction = direction; }
            void setMoveSpeed(const float speed) { m_moveSpeed = speed; }
            void setLookSpeed(const float speed) { m_lookSpeed = speed; }
            void setFov(const float fov) { m_fov = fov; }
            void setNear(const float nearPlane) { m_near = nearPlane; }
            void setFar(const float farPlane) { m_far = farPlane; }

            [[nodiscard]] const std::string &getName() const { return m_name; }
            [[nodiscard]] const glm::vec3 &getPosition() const { return m_position; }
            [[nodiscard]] const glm::vec3 &getRotation() const { return m_rotation; }
            [[nodiscard]] const glm::vec3 &getDirection() const { return m_direction; }
            [[nodiscard]] const float &getMoveSpeed() const { return m_moveSpeed; }
            [[nodiscard]] const float &getLookSpeed() const { return m_lookSpeed; }
            [[nodiscard]] const float &getFov() const { return m_fov; }
            [[nodiscard]] const float &getNear() const { return m_near; }
            [[nodiscard]] const float &getFar() const { return m_far; }

            void move(const glm::vec3 &direction, float deltaTime);
            void rotate(float yawOffset, float pitchOffset, float deltaTime);

        private:
            std::string m_name = Cam::NAME;

            glm::vec3 m_position = glm::vec3(0.0F, 0.0F, 0.0F);
            glm::vec3 m_rotation = glm::vec3(0.0F, 0.0F, 0.0F);
            glm::vec3 m_direction = glm::vec3(0.0F, 0.0F, -1.0F);

            float m_moveSpeed = Cam::MOVE_SPEED;
            float m_lookSpeed = Cam::LOOK_SPEED;

            float m_fov = Cam::FOV;
            float m_near = Cam::NEAR_PLANE;
            float m_far = Cam::FAR_PLANE;

    }; // class Camera

} // namespace cae
