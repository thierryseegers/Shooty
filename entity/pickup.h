#pragma once

#include "entity/entity.h"
#include "entity/leader.h"
#include "scene.h"

#include <SFML/Graphics.hpp>

namespace entity::pickup
{

class pickup : public entity
{
public:
    pickup(
        sf::Texture const& texture);

    virtual void apply(
        leader_t& leader) const = 0;
};

class health : public pickup
{
public:
    health();

    virtual void apply(
        leader_t& leader) const override;
};

class missile_refill : public pickup
{
public:
    missile_refill();

    virtual void apply(
        leader_t& leader) const override;
};

class increase_spread : public pickup
{
public:
    increase_spread();

    virtual void apply(
        leader_t& leader) const override;
};

class increase_fire_rate : public pickup
{
public:
    increase_fire_rate();

    virtual void apply(
        leader_t& leader) const override;
};

}
