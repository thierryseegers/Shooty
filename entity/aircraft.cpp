#include "entity/aircraft.h"

#include "configuration.h"
#include "lifebar.h"

#include <SFML/Graphics.hpp>

#include <memory>

namespace entity
{

aircraft_t::aircraft_t(
    int const starting_life,
    sf::Texture const& texture)
    : entity{starting_life}
    , scene::sprite_t(texture)
{
    auto const bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

    auto b = std::make_unique<lifebar>(bounds.width);
    bar = b.get();
    attach(std::move(b));
}

void aircraft_t::update_self(
    sf::Time const& dt)
{
    entity::update_self(dt);

    bar->adjust((health() * 100) / starting_life);
    bar->setPosition(0.f, 50.f);
    bar->setRotation(-getRotation());
}

}
