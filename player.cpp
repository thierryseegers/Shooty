#include "player.h"

#include "command.h"
#include "aircraft.h"

#include <SFML/Window.hpp>

#include <iostream>

void player_t::handle_event(
    sf::Event const& event,
    commands_t& commands
    )
{
    if(event.type == sf::Event::KeyPressed &&
       event.key.code == sf::Keyboard::P)
    {
        commands.push(make_command<leader_t>([](aircraft_t& aircraft, sf::Time const&)
        {
            std::cout << aircraft.getPosition().x << ',' << aircraft.getPosition().y << "\n";
        }));
    }
}

void player_t::handle_realtime_input(
    commands_t& commands
    )
{
    float const speed = 200.f;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        commands.push(make_command<leader_t>([=](aircraft_t& aircraft, sf::Time const&)
        {
            aircraft.velocity += {0.f, -speed};
        }));
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        commands.push(make_command<leader_t>([=](aircraft_t& aircraft, sf::Time const&)
        {
            aircraft.velocity += {0.f, speed};
        }));
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        commands.push(make_command<leader_t>([=](aircraft_t& aircraft, sf::Time const&)
        {
            aircraft.velocity += {-speed, 0.f};
        }));
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        commands.push(make_command<leader_t>([=](aircraft_t& aircraft, sf::Time const&)
        {
            aircraft.velocity += {speed, 0.f};
        }));
    }
}