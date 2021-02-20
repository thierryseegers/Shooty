#pragma once

#include "scene.h"

#include <SFML/Graphics.hpp>

#include <algorithm>
namespace entity
{

class entity : public virtual scene::node_t
{
public:
    explicit entity()
        : destroyed{false}
    {}

    virtual ~entity() = default;

    sf::Vector2f velocity;
    bool destroyed;

protected:
    virtual void update_self(
        sf::Time const& dt,
        commands_t& commands) override
    {
        move(velocity * dt.asSeconds());
    }
};

}
