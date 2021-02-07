#include "aircraft.h"

#include <SFML/Graphics.hpp>

aircraft::aircraft(
    aircraft::type const type_,
    sf::Texture const& texture)
    : scene::sprite(texture)
    , type_{type_}
{
    auto const bounds = sprite_.getLocalBounds();
    sprite_.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}
