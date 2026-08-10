#pragma once

namespace Pelvis
{

class Transform
{
public:

    Transform();

    void setPosition(
        float x,
        float y,
        float z
    );

    void setRotation(
        float x,
        float y,
        float z
    );

    void setScale(
        float x,
        float y,
        float z
    );

    float getPositionX() const;
    float getPositionY() const;
    float getPositionZ() const;

    float getRotationX() const;
    float getRotationY() const;
    float getRotationZ() const;

    float getScaleX() const;
    float getScaleY() const;
    float getScaleZ() const;

private:

    float m_positionX;
    float m_positionY;
    float m_positionZ;

    float m_rotationX;
    float m_rotationY;
    float m_rotationZ;

    float m_scaleX;
    float m_scaleY;
    float m_scaleZ;
};

}
