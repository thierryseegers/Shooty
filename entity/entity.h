#pragma once

#include "scene.h"

#include <SFML/Graphics.hpp>

namespace entity
{

template<typename T>
class friendly : public T
{
public:
    using T::T;
};

template<typename T>
class hostile : public T
{
public:
    using T::T;
};

class entity : public scene::sprite_t
{
public:
    explicit entity(
    sf::Texture const& texture)
        : sprite_t{texture}
    {}

    virtual ~entity() = default;

    virtual sf::FloatRect collision_bounds() const override
    {
        return world_transform().transformRect(sprite.getGlobalBounds());
    }

    sf::Vector2f velocity;

protected:
    virtual void update_self(
        sf::Time const& dt,
        commands_t& commands) override
    {
        move(velocity * dt.asSeconds());
    }
};

}
