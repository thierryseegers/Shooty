#include "aircraft.h"

#include "resources.h"

#include <map>

const std::map<aircraft::type, resources::texture> type_to_texture = {
    {aircraft::type::eagle, resources::texture::eagle},
    {aircraft::type::raptor, resources::texture::raptor},
};

aircraft::aircraft(
    aircraft::type const type_,
    resources::textures const& textures)
    : type_{type_}
    , sprite{textures.get(type_to_texture.at(type_))}
{
    auto const bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void aircraft::draw_self(
    sf::RenderTarget& target,
    sf::RenderStates states) const
{
    target.draw(sprite, states);
}