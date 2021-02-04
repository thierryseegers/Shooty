#include "aircraft.h"

aircraft::aircraft(
    aircraft::type const type_)
    : type_{type_}
{}

void aircraft::draw_self(
    sf::RenderTarget& target,
    sf::RenderStates states) const
{
    target.draw(sprite, states);
}