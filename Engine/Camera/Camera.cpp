#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

#include <cmath>

namespace Pelvis
{

Camera::Camera()
    : m_x(0.0f)
    , m_y(1.7f)
    , m_z(3.0f)
    , m_yaw(-90.0f)
    , m_pitch(0.0f)
    , m_fov(70.0f)
    , m_nearPlane(0.1f)
    , m_farPlane(1000.0f)
{
}

void Camera::setPosition(
    float x,
    float y,
    float z
)
{
    m_x = x;
    m_y = y;
    m_z = z;
}

void Camera::setRotation(
    float yaw,
    float pitch
)
{
    m_yaw = yaw;
    m_pitch = pitch;

    if (m_pitch > 89.0f)
        m_pitch = 89.0f;

    if (m_pitch < -89.0f)
        m_pitch = -89.0f;
}

void Camera::move(
    float forward,
    float right,
    float up,
    float deltaTime
)
{
    constexpr float speed = 5.0f;

    const float yawRadians =
        m_yaw * 3.14159265359f / 180.0f;

    const float forwardX =
        std::cos(yawRadians);

    const float forwardZ =
        std::sin(yawRadians);

    const float rightX =
        -std::sin(yawRadians);

    const float rightZ =
        std::cos(yawRadians);

    m_x +=
        (forwardX * forward +
         rightX * right) *
        speed *
        deltaTime;

    m_z +=
        (forwardZ * forward +
         rightZ * right) *
        speed *
        deltaTime;

    m_y +=
        up *
        speed *
        deltaTime;
}

void Camera::rotate(
    float yawOffset,
    float pitchOffset
)
{
    m_yaw += yawOffset;
    m_pitch += pitchOffset;

    if (m_pitch > 89.0f)
        m_pitch = 89.0f;

    if (m_pitch < -89.0f)
        m_pitch = -89.0f;
}

float Camera::getX() const
{
    return m_x;
}

float Camera::getY() const
{
    return m_y;
}

float Camera::getZ() const
{
    return m_z;
}

float Camera::getYaw() const
{
    return m_yaw;
}

float Camera::getPitch() const
{
    return m_pitch;
}

float Camera::getFOV() const
{
    return m_fov;
}

float Camera::getNearPlane() const
{
    return m_nearPlane;
}

float Camera::getFarPlane() const
{
    return m_farPlane;
}

glm::mat4 Camera::getViewMatrix() const
{
    const float yaw =
        glm::radians(m_yaw);

    const float pitch =
        glm::radians(m_pitch);

    glm::vec3 position(
        m_x,
        m_y,
        m_z
    );

    glm::vec3 direction;

    direction.x =
        std::cos(pitch) *
        std::cos(yaw);

    direction.y =
        std::sin(pitch);

    direction.z =
        std::cos(pitch) *
        std::sin(yaw);

    return glm::lookAt(
        position,
        position + glm::normalize(direction),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
}

glm::mat4 Camera::getProjectionMatrix(
    float aspectRatio
) const
{
    return glm::perspective(
        glm::radians(m_fov),
        aspectRatio,
        m_nearPlane,
        m_farPlane
    );
}

}
