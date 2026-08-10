#pragma once

namespace Pelvis
{

class GameplayAction
{
public:

    virtual ~GameplayAction() = default;

    virtual void primary() {}
    virtual void secondary() {}

    virtual void interact() {}
    virtual void reload() {}
    virtual void use() {}

};

}
