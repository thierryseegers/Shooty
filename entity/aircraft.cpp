#include "entity/aircraft.h"

#include "lifebar.h"
#include "resources.h"
#include "scene.h"
#include "utility.h"

#include <SFML/System.hpp>

#include <memory>

namespace entity
{

aircraft_t::aircraft_t(
    int const starting_life,
    resources::texture const& texture,
    sf::IntRect const& texture_rect)
    : entity{texture, texture_rect}
    , starting_life{starting_life}
    , life{starting_life}
{
    utility::center_origin(sprite);

    // Make a lifebar for ourself.
    (bar = attach<lifebar>(sprite.getLocalBounds().width))->setPosition(0.f, 20 + sprite.getLocalBounds().height / 2.f);
}

void aircraft_t::damage(
    int const amount)
{
    life = std::max(0, life - amount);
}

void aircraft_t::update_self(
    sf::Time const& dt,
    commands_t& commands)
{
    if(!life)
    {
        remove = true;
    }
    bar->adjust((life * 100) / starting_life);

    entity::update_self(dt, commands);
}

}
