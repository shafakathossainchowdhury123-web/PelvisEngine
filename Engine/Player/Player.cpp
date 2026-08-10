#include "Player.h"

namespace Pelvis
{

Player::Player() = default;

void Player::initialize()
{
    m_controller.initialize(m_camera);
}

void Player::update(float deltaTime)
{
    m_controller.update(m_camera, deltaTime);
}

Camera& Player::getCamera()
{
    return m_camera;
}

PlayerController& Player::getController()
{
    return m_controller;
}

Inventory& Player::getInventory()
{
    return m_inventory;
}

}
