#pragma once

#include <glm/mat4x4.hpp>

namespace Pelvis
{

class Camera
{
public:

    Camera();

    void setPosition(
        float x,
        float y,
        float z
    );

    void setRotation(
        float yaw,
        float pitch
    );

    void move(
        float forward,
        float right,
        float up,
        float deltaTime
    );

    void rotate(
        float yawOffset,
        float pitchOffset
    );

    float getX() const;
    float getY() const;
    float getZ() const;

    float getYaw() const;
    float getPitch() const;

    float getFOV() const;
    float getNearPlane() const;
    float getFarPlane() const;

    glm::mat4 getViewMatrix() const;

    glm::mat4 getProjectionMatrix(
        float aspectRatio
    ) const;

private:

    float m_x;
    float m_y;
    float m_z;

    float m_yaw;
    float m_pitch;

    float m_fov;
    float m_nearPlane;
    float m_farPlane;
};

}
