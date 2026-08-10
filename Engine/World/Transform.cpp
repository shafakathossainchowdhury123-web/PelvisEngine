#include "Transform.h"

namespace Pelvis
{

Transform::Transform()
    : m_positionX(0.0f)
    , m_positionY(0.0f)
    , m_positionZ(0.0f)
    , m_rotationX(0.0f)
    , m_rotationY(0.0f)
    , m_rotationZ(0.0f)
    , m_scaleX(1.0f)
    , m_scaleY(1.0f)
    , m_scaleZ(1.0f)
{
}

void Transform::setPosition(
    float x,
    float y,
    float z
)
{
    m_positionX = x;
    m_positionY = y;
    m_positionZ = z;
}

void Transform::setRotation(
    float x,
    float y,
    float z
)
{
    m_rotationX = x;
    m_rotationY = y;
    m_rotationZ = z;
}

void Transform::setScale(
    float x,
    float y,
    float z
)
{
    m_scaleX = x;
    m_scaleY = y;
    m_scaleZ = z;
}

float Transform::getPositionX() const { return m_positionX; }
float Transform::getPositionY() const { return m_positionY; }
float Transform::getPositionZ() const { return m_positionZ; }

float Transform::getRotationX() const { return m_rotationX; }
float Transform::getRotationY() const { return m_rotationY; }
float Transform::getRotationZ() const { return m_rotationZ; }

float Transform::getScaleX() const { return m_scaleX; }
float Transform::getScaleY() const { return m_scaleY; }
float Transform::getScaleZ() const { return m_scaleZ; }

}
