#include "entity/leader.h"


#include "configuration.h"
#include "entity/bullet.h"
#include "entity/missile.h"
#include "entity/projectile.h"
#include "resources.h"
#include "scene.h"
#include "utility.h"

#include <SFML/System.hpp>

namespace entity
{

leader_t::leader_t()
    : friendly<aircraft_t>{*utility::single::instance<configuration::values>()["leader"]["starting_health"].value<int>(),
                           utility::single::instance<resources::textures>().get(resources::texture::eagle)}
    , fire_rate{1}
    , bullet_spread{1}
    , fire_countdown{sf::Time::Zero}
    , firing{false}
    , missile_guidance{false}
    , missile_ammo{10}
    , launching_missile{false}
{}

void leader_t::fire()
{
    firing = true;
}

void leader_t::launch_missile()
{
    if(missile_ammo > 0 && !launching_missile)
    {
        launching_missile = true;
        --missile_ammo;
    }
}

void leader_t::repair(
    int const amount)
{
    life = std::min(life + amount, starting_life);
}

void leader_t::increase_fire_rate()
{
    fire_rate = std::max(10, fire_rate + 1);
}

void leader_t::increase_bullet_spread()
{
    bullet_spread = std::min(3, bullet_spread + 1);
}

void leader_t::collect_missile(
    int const amount)
{
    missile_ammo += 3;
}

void leader_t::update_self(
        sf::Time const& dt,
        commands_t& commands)
{
    if(firing && fire_countdown <= sf::Time::Zero)
    {
        commands.push(make_command<scene::air>([=](scene::air& air, sf::Time const&)
        {
            shoot_bullet(air);
        }));

        fire_countdown += sf::seconds(1.f / (fire_rate + 1));
        firing = false;
    }
    else if(fire_countdown > sf::Time::Zero)
    {
        fire_countdown -= dt;
        firing = false;
    }

    if(launching_missile)
    {
        commands.push(make_command<scene::air>([=](scene::air& air, sf::Time const&)
        {
            shoot_missile(air);
        }));
        launching_missile = false;
    }

    aircraft_t::update_self(dt, commands);
}

void leader_t::shoot_bullet(
    scene::air& air) const
{
    switch(bullet_spread)
    {
    case 1:
        add_projectile<bullet<friendly>>(air, {0.f, 0.5f}, projectile::upward);
        break;
    case 2:
        add_projectile<bullet<friendly>>(air, {-0.33f, 0.33f}, projectile::upward);
        add_projectile<bullet<friendly>>(air, {0.33f, 0.33f}, projectile::upward);
        break;
    case 3:
        add_projectile<bullet<friendly>>(air, {0.f, 0.5f}, projectile::upward);
        add_projectile<bullet<friendly>>(air, {-0.33f, 0.33f}, projectile::upward);
        add_projectile<bullet<friendly>>(air, {0.33f, 0.33f}, projectile::upward);
        break;
    }
}

void leader_t::shoot_missile(scene::air& air) const
{
    if(missile_guidance)
    {
        add_projectile<guided_missile<friendly>>(air, {0.f, 0.5f}, projectile::upward);
    }
    else
    {
        add_projectile<missile<friendly>>(air, {0.f, 0.5f}, projectile::upward);
    }
}

}
