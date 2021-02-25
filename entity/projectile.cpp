#include "projectile.h"

#include "resources.h"
#include "utility.h"

#include <SFML/Graphics/Rect.hpp>
namespace entity
{

projectile::projectile(
    float const speed,
    int const damage,
    resources::texture const& texture,
    sf::IntRect const& texture_rect)
    : entity{texture, texture_rect}
    , damage{damage}
    , speed{speed}
{
    utility::center_origin(sprite);
}

}