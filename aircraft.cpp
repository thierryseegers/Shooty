#include "aircraft.h"

#include <SFML/Graphics.hpp>

aircraft_t::aircraft_t(
    aircraft_t::type const type_,
    sf::Texture const& texture)
    : scene::sprite_t(texture)
    , type_{type_}
{
    auto const bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}
