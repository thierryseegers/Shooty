#include "missile.h"

#include "configuration.h"
#include "resources.h"
#include "utility.h"

#include <cmath>

namespace entity
{

missile::missile()
    : projectile{*utility::single::instance<configuration::values>()["missile"]["speed"].value<float>(),
                 *utility::single::instance<configuration::values>()["missile"]["damage"].value<int>(),
                 utility::single::instance<resources::textures>().get(resources::texture::missile)}
{}

guided_missile::guided_missile()
    : missile{}
{}

void guided_missile::guide(
    sf::Vector2f const& position)
{
    direction = utility::unit(position - world_position());
}

void guided_missile::update_self(
    sf::Time const& dt,
    commands_t& commands)
{
    float const approach_rate = 200.f;

    auto const v = utility::unit(approach_rate * dt.asSeconds() * direction + velocity) * speed;
    auto const angle = std::atan2(v.y, v.x);

    setRotation(utility::to_degree(angle) + 90.f);
    velocity = v;

    missile::update_self(dt, commands);
}

}
