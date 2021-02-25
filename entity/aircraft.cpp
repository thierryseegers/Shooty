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
    auto b = std::make_unique<lifebar>(sprite.getLocalBounds().width);
    bar = b.get();
    attach(std::move(b));
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

    entity::update_self(dt, commands);

    bar->adjust((health() * 100) / starting_life);
    bar->setPosition(0.f, 50.f);
    bar->setRotation(-getRotation());
}

}
