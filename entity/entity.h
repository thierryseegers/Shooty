#pragma once

#include "command.h"
#include "scene.h"
#include "resources.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

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
        resources::texture const& texture,
        sf::IntRect const texture_rect)
        : sprite_t{texture, texture_rect}
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
