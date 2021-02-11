#pragma once

#include "scene.h"

#include <SFML/Graphics.hpp>

struct entity : public virtual scene::node_t
{
    virtual void update_self(
        sf::Time const dt) override
    {
        move(velocity * dt.asSeconds());
    }

    sf::Vector2f velocity;
};
