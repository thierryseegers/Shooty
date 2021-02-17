#pragma once

#include "entity/aircraft.h"
#include "entity/flight.h"
#include "resources.h"
#include "utility.h"

#include <SFML/Graphics.hpp>

#include <vector>

namespace entity
{


class enemy : public aircraft_t
{
public:
    enemy(
        int const starting_life,
        int const speed,
        sf::Texture const& texture,
        std::vector<flight::direction> const& pattern);

    virtual void update_self(
        sf::Time const& dt) override;

protected:
    int const speed;

    std::vector<flight::direction> const pattern;
    utility::cyclic_iterator<decltype(pattern)> current;

    float travelled;
};

class avenger : public enemy
{
public:
    avenger(
        resources::textures const& textures);
};

class raptor : public enemy
{
public:
    raptor(
        resources::textures const& textures);
};

}