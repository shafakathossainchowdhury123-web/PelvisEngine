#pragma once

#include "PlayerController.h"

#include "../Camera/Camera.h"
#include "../Gameplay/Inventory.h"

namespace Pelvis
{

class Player
{
public:

    Player();

    void initialize();

    void update(float deltaTime);

    Camera& getCamera();

    PlayerController& getController();

    Inventory& getInventory();

private:

    Camera m_camera;

    PlayerController m_controller;

    Inventory m_inventory;
};

}
