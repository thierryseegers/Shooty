#include "projectile.h"

#include "utility.h"

namespace entity
{

projectile::projectile(
    float const speed,
    int const damage,
    sf::Texture const& texture)
    : sprite_t{texture}
    , damage{damage}
    , speed{speed}
{
    utility::center_origin(sprite);
}

}