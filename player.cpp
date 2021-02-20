#include "player.h"

#include "command.h"
#include "entity/aircraft.h"
#include "entity/leader.h"

#include <SFML/Window.hpp>

#include <map>

player_t::player_t()
{
    bind_key(sf::Keyboard::Down, action::move_down);
    bind_key(sf::Keyboard::Left, action::move_left);
    bind_key(sf::Keyboard::Right, action::move_right);
    bind_key(sf::Keyboard::Up, action::move_up);
    bind_key(sf::Keyboard::Space, action::fire);
    bind_key(sf::Keyboard::M, action::launch_missile);

    float const speed = 200.f;

    action_bindings[action::move_down] = make_command<entity::leader_t>([=](entity::aircraft_t& aircraft, sf::Time const&)
        {
            aircraft.velocity += {0.f, speed};
        });
    action_bindings[action::move_left] = make_command<entity::leader_t>([=](entity::aircraft_t& aircraft, sf::Time const&)
        {
            aircraft.velocity += {-speed, 0.f};
        });
    action_bindings[action::move_right] = make_command<entity::leader_t>([=](entity::aircraft_t& aircraft, sf::Time const&)
        {
            aircraft.velocity += {speed, 0.f};
        });
    action_bindings[action::move_up] = make_command<entity::leader_t>([=](entity::aircraft_t& aircraft, sf::Time const&)
        {
            aircraft.velocity += {0.f, -speed};
        });

    action_bindings[action::fire] = make_command<entity::leader_t>([=](entity::leader_t& leader, sf::Time const&)
        {
            leader.fire();
        });
    action_bindings[action::launch_missile] = make_command<entity::leader_t>([=](entity::leader_t& leader, sf::Time const&)
        {
            leader.launch_missile();
        });
}

void player_t::bind_key(
    sf::Keyboard::Key const key,
    action const what)
{
    // Remove all keys that already map to the action.
    for(auto i = key_bindings.begin(); i != key_bindings.end();)
    {
        if(what == i->second)
        {
            key_bindings.erase(i++);
        }
        else
        {
            ++i;
        }
    }

    key_bindings[key] = what;
}

sf::Keyboard::Key player_t::bound_key(
    action const what) const
{
    for(auto const [key, a] : key_bindings)
    {
        if(a == what)
        {
            return key;
        }
    }

    return sf::Keyboard::Unknown;
}

void player_t::handle_event(
    sf::Event const& event,
    commands_t& commands)
{
    if(event.type == sf::Event::KeyPressed)
    {
        if(auto i = key_bindings.find(event.key.code); i != key_bindings.end() && !is_realtime_action(i->second))
        {
            commands.push(action_bindings[i->second]);
        }
    }
}

void player_t::handle_realtime_input(
    commands_t& commands)
{
    for(auto const [key, what] : key_bindings)
    {
        if(sf::Keyboard::isKeyPressed(key) &&
           is_realtime_action(what))
        {
            commands.push(action_bindings[what]);
        }
    }
}

bool player_t::is_realtime_action(
    action const a)
{
    switch(a)
    {
        case action::move_down:
        case action::move_left:
        case action::move_right:
        case action::move_up:
        case action::fire:
            return true;
            break;
        default:
            return false;
            break;
    }
}