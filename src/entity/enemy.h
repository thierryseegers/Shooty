#pragma once

#include "command.h"
#include "entity/aircraft.h"
#include "entity/entity.h"
#include "entity/flight.h"
#include "resources.h"
#include "scene.h"
#include "utility.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Time.hpp>

#include <vector>

namespace entity
{


class enemy : public hostile<aircraft_t>
{
public:
    enemy(
        int const starting_life,
        int const speed,
        float const attack_rate,
        flight::pattern const& pattern,
        resources::texture const& texture,
        sf::IntRect const& text_rect);

    virtual ~enemy() = default;

protected:
    virtual void update_self(
        sf::Time const& dt,
        commands_t& commands) override;

    virtual void attack(
        scene::projectiles& layer) const = 0;

    int const speed;

    flight::pattern const pattern;
    utility::cyclic_iterator<decltype(pattern)> current;

    float travelled;

    float const attack_rate;
    sf::Time attack_countdown;
};

class avenger : public enemy
{
public:
    avenger();

protected:
    virtual void attack(
        scene::projectiles& layer) const override;
};

class raptor : public enemy
{
public:
    raptor();

protected:
    virtual void attack(
        scene::projectiles& layer) const override;
};

}