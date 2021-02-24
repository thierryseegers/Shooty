#include "entity/enemy.h"

#include "configuration.h"
#include "entity/bullet.h"
#include "entity/entity.h"
#include "entity/flight.h"
#include "entity/missile.h"
#include "entity/pickup.h"
#include "resources.h"
#include "scene.h"
#include "utility.h"

#include <SFML/Graphics.hpp>

#include <memory>
#include <vector>

namespace entity
{

std::vector<flight::direction> to_pattern(toml::array const& values)
{
    std::vector<flight::direction> pattern;

    for(auto const& node : values)
    {
        auto const& move = *node.as_array();
        pattern.push_back({*move[0].value<float>(), *move[1].value<float>()});
    }

    return pattern;
}

enemy::enemy(
    int const starting_life,
    int const speed,
    float const attack_rate,
    sf::Texture const& texture,
    std::vector<flight::direction> const& pattern)
    : hostile<aircraft_t>{starting_life, texture}
    , speed{speed}
    , attack_rate{attack_rate}
    , pattern{pattern}
    , current{this->pattern, this->pattern.begin()}
    , travelled{0}
{}

void enemy::update_self(
        sf::Time const& dt,
        commands_t& commands)
{
    if(life)
    {
        // Update travel.
        if(travelled > current->distance)
        {
            ++current;
            travelled = 0;
        }

        float const rad = utility::to_radian(current->angle + 90.f);
        velocity = {speed * std::cos(rad), speed * std::sin(rad)};

        travelled += speed * dt.asSeconds();

        // Update attack.
        if(attack_countdown <= sf::Time::Zero)
        {
            commands.push(make_command<scene::air>([=](scene::air& air, sf::Time const&)
            {
                attack(air);
            }));

            attack_countdown += sf::seconds(1.f / attack_rate);
        }
        else
        {
            attack_countdown -= dt;
        }
    }
    else if(!remove)
    {
        if(utility::random(2) == 0)
        {
            commands.push(make_command<scene::air>([=](scene::air& air, sf::Time const&)
            {
                std::unique_ptr<pickup::pickup> pickup;
                switch(utility::random(3))
                {
                    case 0:
                        pickup = std::make_unique<pickup::health>();
                        break;
                    case 1:
                        pickup = std::make_unique<pickup::missile_refill>();
                        break;
                    case 2:
                        pickup = std::make_unique<pickup::increase_spread>();
                        break;
                    case 3:
                        pickup = std::make_unique<pickup::increase_fire_rate>();
                        break;
                }

                pickup->setPosition(world_position());
                pickup->velocity = {0.f, 1.f};
                air.attach(std::move(pickup));
            }));
        }

        remove = true;
    }

    aircraft_t::update_self(dt, commands);
}

avenger::avenger()
    : enemy{*utility::single::instance<configuration::values>()["avenger"]["starting_health"].value<int>(),
            *utility::single::instance<configuration::values>()["avenger"]["speed"].value<int>(),
            *utility::single::instance<configuration::values>()["avenger"]["attack_rate"].value<float>(),
            utility::single::instance<resources::textures>().get(resources::texture::avenger),
            to_pattern(*utility::single::instance<configuration::values>()["avenger"]["flight_pattern"].as_array())}
{}

void avenger::attack(
    scene::air& air) const
{
    add_projectile<bullet<hostile>>(air, {0.f, 0.5f}, projectile::downward);
}

raptor::raptor()
    : enemy{*utility::single::instance<configuration::values>()["raptor"]["starting_health"].value<int>(),
            *utility::single::instance<configuration::values>()["raptor"]["speed"].value<int>(),
            *utility::single::instance<configuration::values>()["raptor"]["attack_rate"].value<float>(),
            utility::single::instance<resources::textures>().get(resources::texture::raptor),
            to_pattern(*utility::single::instance<configuration::values>()["raptor"]["flight_pattern"].as_array())}
{}

void raptor::attack(
    scene::air& air) const
{
    add_projectile<missile<hostile>>(air, {0.f, 0.5f}, projectile::downward);
}

}
