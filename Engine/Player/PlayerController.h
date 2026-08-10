#pragma once

#include "../Camera/Camera.h"
#include "../Input/Input.h"

namespace Pelvis
{

class PlayerController
{
public:

    PlayerController();

    void initialize(Camera& camera);

    void update(Camera& camera, float deltaTime);

    float getMoveSpeed() const;
    float getSprintMultiplier() const;

private:

    void processMovement(Camera& camera, float deltaTime);
    void processLook(Camera& camera);

    float m_moveSpeed;
    float m_sprintMultiplier;
    float m_mouseSensitivity;

    float m_verticalVelocity;
    float m_gravity;

    bool m_grounded;
    bool m_crouching;
};

}
