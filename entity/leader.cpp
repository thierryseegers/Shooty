#include "entity/leader.h"

#include "configuration.h"

namespace entity
{

leader_t::leader_t(
    resources::textures const& textures)
    : aircraft_t{*configuration::instance()["leader"]["starting_health"].value<int>(),
                 textures.get(resources::texture::eagle)}
{}

}