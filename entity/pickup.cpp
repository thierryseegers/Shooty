#include "pickup.h"

#include "entity/leader.h"
#include "resources.h"
#include "utility.h"

#include <SFML/Graphics.hpp>

namespace entity::pickup
{

pickup::pickup(
    sf::Texture const& texture)
    : entity{texture}
{}

health::health()
    : pickup{utility::single::instance<resources::textures>().get(resources::texture::health)}
{}

void health::apply(
    leader_t& leader) const
{
    leader.repair(25);
}

missile_refill::missile_refill()
    : pickup{utility::single::instance<resources::textures>().get(resources::texture::missile_refill)}
{}

void missile_refill::apply(
    leader_t& leader) const
{
    leader.collect_missile(3);
}

increase_spread::increase_spread()
    : pickup{utility::single::instance<resources::textures>().get(resources::texture::increase_spread)}
{}

void increase_spread::apply(
    leader_t& leader) const
{
    leader.increase_bullet_spread();
}

increase_fire_rate::increase_fire_rate()
    : pickup{utility::single::instance<resources::textures>().get(resources::texture::increase_fire_rate)}
{}

void increase_fire_rate::apply(
    leader_t& leader) const
{
    leader.increase_fire_rate();
}

}
