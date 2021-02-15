#pragma once

#include "scene.h"

#include <SFML/Graphics.hpp>

namespace entity
{

class entity : public virtual scene::node_t
{
public:
    explicit entity(
        int const life)
        : life{life}
    {}

    void heal(
        int const amount)
    {
        if(alive())
        {
            life += amount;
        }
    }

    void hurt(
        int const amount)
    {
        if(alive())
        {
            life -= amount;
        }
    }

    void destroy()
    {
        life = 0;
    }

    [[nodiscard]] int health() const
    {
        return life;
    }

    [[nodiscard]] bool alive() const
    {
        return life > 0;
    }

    virtual void update_self(
        sf::Time const& dt) override
    {
        move(velocity * dt.asSeconds());
    }

    sf::Vector2f velocity;

private:
    int life;
};

}
