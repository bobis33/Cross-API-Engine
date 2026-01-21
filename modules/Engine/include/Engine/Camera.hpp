///
/// @file Camera.hpp
/// @brief This file contains the camera class declaration
/// @namespace cae
///

#pragma once

#include "Engine/Common.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <string>

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
            Camera(const glm::vec3 position, const glm::vec3 rotation, const glm::vec3 direction,
                   const float moveSpeed = CAMERA::MOVE_SPEED, const float lookSpeed = CAMERA::LOOK_SPEED,
                   const float fov = CAMERA::FOV, const float nearPlane = CAMERA::NEAR_PLANE,
                   const float farPlane = CAMERA::FAR_PLANE)
                : m_position(position), m_rotation(rotation), m_direction(direction), m_moveSpeed(moveSpeed),
                  m_lookSpeed(lookSpeed), m_fov(fov), m_near(nearPlane), m_far(farPlane)
            {
            }
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

            [[nodiscard]] glm::mat4 getViewMatrix() const
            {
                return glm::lookAt(m_position, m_position + m_direction, glm::vec3(0.0F, 1.0F, 0.0F));
            }
            [[nodiscard]] glm::mat4 getProjectionMatrix(const float aspectRatio) const
            {
                return glm::perspective(glm::radians(m_fov), aspectRatio, m_near, m_far);
            }
            [[nodiscard]] glm::mat4 getViewProjection(const float aspectRatio) const
            {
                return getProjectionMatrix(aspectRatio) * getViewMatrix();
            }

            ///
            /// @param direction Direction to move the camera
            /// @param deltaTime Time delta for movement
            /// @brief Move the camera in a given direction
            ///
            void move(const glm::vec3 &direction, const float deltaTime)
            {
                m_position += direction * m_moveSpeed * deltaTime;
            }

            ///
            /// @param yawOffset Yaw offset to rotate the camera
            /// @param pitchOffset Pitch offset to rotate the camera
            /// @param deltaTime Time delta for rotation
            /// @brief Rotate the camera by given yaw and pitch offsets
            ///
            void rotate(const float yawOffset, const float pitchOffset, const float deltaTime)
            {
                m_rotation.y += yawOffset * m_lookSpeed * deltaTime;
                m_rotation.x += pitchOffset * m_lookSpeed * deltaTime;

                m_rotation.x = std::min(m_rotation.x, 89.0F);
                m_rotation.x = std::max(m_rotation.x, -89.0F);

                updateDirectionFromRotation();
            }

        private:
            std::string m_name = CAMERA::NAME;

            glm::vec3 m_position = glm::vec3(0.0F, 0.0F, 0.0F);
            glm::vec3 m_rotation = glm::vec3(0.0F, 0.0F, 0.0F);
            glm::vec3 m_direction = glm::vec3(0.0F, 0.0F, -1.0F);

            float m_moveSpeed = CAMERA::MOVE_SPEED;
            float m_lookSpeed = CAMERA::LOOK_SPEED;

            float m_fov = CAMERA::FOV;
            float m_near = CAMERA::NEAR_PLANE;
            float m_far = CAMERA::FAR_PLANE;

            void updateDirectionFromRotation()
            {
                const float yaw = glm::radians(m_rotation.y);
                const float pitch = glm::radians(m_rotation.x);

                m_direction.x = cos(pitch) * sin(yaw);
                m_direction.y = std::sin(pitch);
                m_direction.z = -cos(pitch) * cos(yaw);
                m_direction = glm::normalize(m_direction);
            }

    }; // class Camera

} // namespace cae
