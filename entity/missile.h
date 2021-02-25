#pragma once

#include "command.h"
#include "configuration.h"
#include "entity/entity.h"
#include "entity/projectile.h"
#include "resources.h"
#include "utility.h"

#include <magic_enum.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Vector2.hpp>

#include <cmath>

namespace entity
{

template<template<class> class Kind>
class missile : public Kind<projectile>
{
public:
    missile()
        : Kind<projectile>{*configuration::values()["projectile"]["missile"]["speed"].value<float>(),
                           *configuration::values()["projectile"]["missile"]["damage"].value<int>(),
                           *magic_enum::enum_cast<resources::texture>(*configuration::values()["projectile"]["texture"].value<std::string_view>()),
                           utility::to_intrect(*configuration::values()["projectile"]["missile"]["texture_rect"].as_array())}
{}

    virtual ~missile() = default;
};

template<template<class> class Kind>
class guided_missile : public missile<Kind>
{
public:
    guided_missile()
        : missile<Kind>{}
    {}

    virtual ~guided_missile() = default;

    void guide(sf::Vector2f const& position)
    {
       direction = utility::unit(position - entity::world_position());
    }

private:
    virtual void update_self(
        sf::Time const& dt,
        commands_t& commands) override
    {
        float const approach_rate = 200.f;

        auto const v = utility::unit(approach_rate * dt.asSeconds() * direction + entity::velocity) * projectile::speed;
        auto const angle = std::atan2(v.y, v.x);

        sf::Transformable::setRotation(utility::to_degree(angle) + 90.f);
        entity::velocity = v;

        entity::update_self(dt, commands);
    }

    sf::Vector2f direction;
};

}
