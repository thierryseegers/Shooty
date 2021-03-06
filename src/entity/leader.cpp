#include "entity/leader.h"

#include "command.h"
#include "configuration.h"
#include "entity/aircraft.h"
#include "entity/bullet.h"
#include "entity/entity.h"
#include "entity/missile.h"
#include "entity/projectile.h"
#include "resources.h"
#include "scene.h"
#include "utility.h"

#include <magic_enum.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Time.hpp>

#include <algorithm>
#include <string>

namespace entity
{

leader_t::leader_t()
    : friendly<aircraft_t>{*configuration::values()["aircraft"]["leader"]["starting_health"].value<int>(),
                           *magic_enum::enum_cast<resources::texture>(*configuration::values()["aircraft"]["texture"].value<std::string>()),
                           utility::to_intrect(*configuration::values()["aircraft"]["leader"]["texture_rect"].as_array())}
    , default_texture_rect{sprite.getTextureRect()}
    , bullet_spread{1}
    , fire_rate{1}
    , fire_countdown{sf::Time::Zero}
    , firing{false}
    , missile_ammo{10}
    // , missile_guidance{false}
    , missile_guidance{true}
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
    missile_ammo += amount;
}

void leader_t::update_self(
        sf::Time const& dt,
        commands_t& commands)
{
    {
        auto texture_rect = default_texture_rect;
        if(velocity.x < 0.f)
        {
            texture_rect.left += default_texture_rect.width;
        }
        else if(velocity.x > 0.f)
        {
            texture_rect.left += 2 * default_texture_rect.width;
        }

        sprite.setTextureRect(texture_rect);
    }

    if(firing && fire_countdown <= sf::Time::Zero)
    {
        commands.push(make_command<scene::projectiles>([=](scene::projectiles& layer, sf::Time const&)
        {
            shoot_bullet(layer);
        }));

        play_local_sound(commands, resources::sound_effect::allied_gunfire);

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
        commands.push(make_command<scene::projectiles>([=](scene::projectiles& layer, sf::Time const&)
        {
            shoot_missile(layer);
        }));

        play_local_sound(commands, resources::sound_effect::launch_missile);

        launching_missile = false;
    }

    aircraft_t::update_self(dt, commands);
}

void leader_t::shoot_bullet(
    scene::projectiles& layer) const
{
    switch(bullet_spread)
    {
    case 1:
        add_projectile<bullet<friendly>>(layer, {0.f, 0.5f}, projectile::upward);
        break;
    case 2:
        add_projectile<bullet<friendly>>(layer, {-0.33f, 0.33f}, projectile::upward);
        add_projectile<bullet<friendly>>(layer, {0.33f, 0.33f}, projectile::upward);
        break;
    case 3:
        add_projectile<bullet<friendly>>(layer, {0.f, 0.5f}, projectile::upward);
        add_projectile<bullet<friendly>>(layer, {-0.33f, 0.33f}, projectile::upward);
        add_projectile<bullet<friendly>>(layer, {0.33f, 0.33f}, projectile::upward);
        break;
    }
}

void leader_t::shoot_missile(
    scene::projectiles& layer) const
{
    if(missile_guidance)
    {
        add_projectile<guided_missile<friendly>>(layer, {0.f, 0.5f}, projectile::upward);
    }
    else
    {
        add_projectile<missile<friendly>>(layer, {0.f, 0.5f}, projectile::upward);
    }
}

}
