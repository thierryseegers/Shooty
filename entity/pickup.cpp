#include "pickup.h"

#include "configuration.h"
#include "entity/leader.h"
#include "resources.h"
#include "utility.h"

#include <magic_enum.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <string>

namespace entity::pickup
{

pickup::pickup(
    resources::texture const& texture,
    sf::IntRect const& texture_rect)
    : entity{texture, texture_rect}
{
    utility::center_origin(sprite);
}

health::health()
    : pickup{*magic_enum::enum_cast<resources::texture>(*configuration::values()["pickup"]["texture"].value<std::string>()),
             utility::to_intrect(*configuration::values()["pickup"]["health"]["texture_rect"].as_array())}
{}

void health::apply(
    leader_t& leader) const
{
    leader.repair(25);
}

missile_refill::missile_refill()
    : pickup{*magic_enum::enum_cast<resources::texture>(*configuration::values()["pickup"]["texture"].value<std::string>()),
             utility::to_intrect(*configuration::values()["pickup"]["missile_refill"]["texture_rect"].as_array())}
{}

void missile_refill::apply(
    leader_t& leader) const
{
    leader.collect_missile(3);
}

increase_spread::increase_spread()
    : pickup{*magic_enum::enum_cast<resources::texture>(*configuration::values()["pickup"]["texture"].value<std::string>()),
             utility::to_intrect(*configuration::values()["pickup"]["fire_spread"]["texture_rect"].as_array())}
{}

void increase_spread::apply(
    leader_t& leader) const
{
    leader.increase_bullet_spread();
}

increase_fire_rate::increase_fire_rate()
    : pickup{*magic_enum::enum_cast<resources::texture>(*configuration::values()["pickup"]["texture"].value<std::string>()),
             utility::to_intrect(*configuration::values()["pickup"]["fire_rate"]["texture_rect"].as_array())}
{}

void increase_fire_rate::apply(
    leader_t& leader) const
{
    leader.increase_fire_rate();
}

}
