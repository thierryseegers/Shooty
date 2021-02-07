#include "game.h"

#include "resources.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <filesystem>
#include <stdexcept>

game::game()
    : window{sf::VideoMode(640, 480), "Shooty"}
    , world_{window}
{}

void game::run()
{
    sf::Clock clock;
    sf::Time last_update = sf::Time::Zero;

     while(window.isOpen())
    {
        last_update += clock.restart();
        while(last_update > time_per_frame)
        {
            last_update -= time_per_frame;
            process_events();
            update(time_per_frame);
        }

        render();
    }
}

void game::process_events()
{
    sf::Event event;
    while(window.pollEvent(event))
    {
        switch(event.type)
        {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                handle_player_input(event.key.code, true);
                break;
            case sf::Event::KeyReleased:
                handle_player_input(event.key.code, false);
                break;
            default:
                break;
        }
    }
}

void game::handle_player_input(
    sf::Keyboard::Key const key,
    bool const is_pressed)
{}

void game::update(
    sf::Time const& dt)
{
    world_.update(dt);
}

void game::render()
{
    window.clear();
    world_.draw();
    
    window.setView(window.getDefaultView());
    window.display();
}