#include "game.h"

#include <SFML/Graphics.hpp>

game::game()
    : window{sf::VideoMode(640, 480), "Shooty"}
{
    player.setRadius(40.f);
    player.setPosition(100.f, 100.f);
    player.setFillColor(sf::Color::Cyan);
}

void game::run()
{
     while(window.isOpen())
    {
        process_events();
        update();
        render();
    }
}

void game::process_events()
{
    sf::Event event;
    while(window.pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
        {
            window.close();
        }
    }
}

void game::update()
{
}

void game::render()
{
    window.clear();
    window.draw(player);
    window.display();
}