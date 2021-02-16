#pragma once

#include "scene.h"

#include <SFML/Graphics.hpp>

#include <algorithm>
namespace entity
{

class entity : public virtual scene::node_t
{
public:
    explicit entity(
        int const starting_life)
        : starting_life{starting_life}
        , life{starting_life}
    {}

    void heal(
        int const amount)
    {
        if(alive())
        {
            life = std::max(life + amount, starting_life);
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

    int const starting_life;

private:
    int life;
};

}
