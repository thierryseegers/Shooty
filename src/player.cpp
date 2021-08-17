#include "player.h"

#include "action.h"
#include "bindings.h"
#include "command.h"
#include "entity/aircraft.h"
#include "entity/leader.h"

#include <SFML/Window.hpp>

#include <map>

player_t::player_t()
    : status{mission::running}
{
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

void player_t::handle_event(
    sf::Event const& event,
    commands_t& commands)
{
    if(event.type == sf::Event::KeyPressed)
    {
        if(auto i = bindings::keyboard().find(event.key.code); i != bindings::keyboard().end() && !is_realtime_action(i->second))
        {
            commands.push(action_bindings[i->second]);
        }
    }
    else if(event.type == sf::Event::JoystickButtonPressed)
    {
        if(auto i = bindings::joystick().find(event.joystickButton.button); i != bindings::joystick().end() && !is_realtime_action(i->second))
        {
            commands.push(action_bindings[i->second]);
        }
    }
}

void player_t::handle_realtime_input(
    commands_t& commands)
{
    for(auto const& [key, what] : bindings::keyboard())
    {
        if(sf::Keyboard::isKeyPressed(key) &&
           is_realtime_action(what))
        {
            commands.push(action_bindings[what]);
        }
    }

    for(auto const& [button, what] : bindings::joystick())
    {
        if(sf::Joystick::isButtonPressed(0, button) &&
           is_realtime_action(what))
        {
            commands.push(action_bindings[what]);
        }
    }

    if(auto const x_axis = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X); x_axis > 1)
    {
        commands.push(action_bindings[action::move_right]);
    }
    else if(x_axis < -1)
    {
        commands.push(action_bindings[action::move_left]);
    }

    if(auto const y_axis = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y); y_axis > 1)
    {
        commands.push(action_bindings[action::move_down]);
    }
    else if(y_axis < -1)
    {
        commands.push(action_bindings[action::move_up]);
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