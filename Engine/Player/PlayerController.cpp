#include "PlayerController.h"

#include <cmath>

namespace Pelvis
{

PlayerController::PlayerController()
    : m_moveSpeed(5.0f)
    , m_sprintMultiplier(2.0f)
    , m_mouseSensitivity(0.10f)
    , m_verticalVelocity(0.0f)
    , m_gravity(-20.0f)
    , m_grounded(true)
    , m_crouching(false)
{
}

void PlayerController::initialize(Camera& camera)
{
    (void)camera;

    m_verticalVelocity = 0.0f;
    m_grounded = true;
    m_crouching = false;
}

void PlayerController::update(
    Camera& camera,
    float deltaTime
)
{
    processMovement(
        camera,
        deltaTime
    );

    processLook(camera);
}

void PlayerController::processMovement(
    Camera& camera,
    float deltaTime
)
{
    float forward = 0.0f;
    float right = 0.0f;

    if (Input::isKeyDown(Key::W))
        forward += 1.0f;

    if (Input::isKeyDown(Key::S))
        forward -= 1.0f;

    if (Input::isKeyDown(Key::D))
        right += 1.0f;

    if (Input::isKeyDown(Key::A))
        right -= 1.0f;

    /*
        Normalize diagonal movement.
    */

    const float lengthSquared =
        forward * forward +
        right * right;

    if (lengthSquared > 1.0f)
    {
        const float scale =
            1.0f / std::sqrt(lengthSquared);

        forward *= scale;
        right *= scale;
    }

    /*
        Sprint
    */

    float speed = m_moveSpeed;

    if (Input::isKeyDown(Key::LeftShift))
    {
        speed *= m_sprintMultiplier;
    }

    /*
        Crouch
    */

    m_crouching =
        Input::isKeyDown(Key::LeftControl);

    if (m_crouching)
    {
        speed *= 0.5f;
    }

    /*
        Horizontal movement.
    */

    camera.move(
        forward * (speed / 5.0f),
        right * (speed / 5.0f),
        0.0f,
        deltaTime
    );

    /*
        Jump
    */

    if (Input::isKeyPressed(Key::Space) && m_grounded)
    {
        m_verticalVelocity = 8.0f;
        m_grounded = false;
    }

    /*
        Gravity.
    */

    if (!m_grounded)
    {
        m_verticalVelocity +=
            m_gravity * deltaTime;

        camera.move(
            0.0f,
            0.0f,
            m_verticalVelocity / 5.0f,
            deltaTime
        );

        /*
            Temporary ground height.

            Real collision detection will replace
            this later when the physics system exists.
        */

        if (camera.getY() <= 1.7f)
        {
            camera.setPosition(
                camera.getX(),
                1.7f,
                camera.getZ()
            );

            m_verticalVelocity = 0.0f;
            m_grounded = true;
        }
    }
}

void PlayerController::processLook(Camera& camera)
{
    const float mouseX =
        Input::getMouseDeltaX();

    const float mouseY =
        Input::getMouseDeltaY();

    camera.rotate(
        mouseX * m_mouseSensitivity,
        -mouseY * m_mouseSensitivity
    );
}

float PlayerController::getMoveSpeed() const
{
    return m_moveSpeed;
}

float PlayerController::getSprintMultiplier() const
{
    return m_sprintMultiplier;
}

}
