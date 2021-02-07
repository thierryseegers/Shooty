#include "game.h"

#include "resources.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <filesystem>
#include <stdexcept>

game::game()
    : window{sf::VideoMode(640, 480), "Shooty"}
{
    textures.load(resources::texture::landscape, "Book/02_Resources/Media/Textures/Desert.png");
    textures.load(resources::texture::eagle, "Book/02_Resources/Media/Textures/Eagle.png");

    landscape.setTexture(textures.get(resources::texture::landscape));
    player.setTexture(textures.get(resources::texture::eagle));
    player.setPosition(100.f, 100.f);
}

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
{
    switch(key)
    {
        case sf::Keyboard::W:
        case sf::Keyboard::Up:
            moving_up = is_pressed;
            break;
        case sf::Keyboard::A:
        case sf::Keyboard::Left:
            moving_left = is_pressed;
            break;
        case sf::Keyboard::S:
        case sf::Keyboard::Down:
            moving_down = is_pressed;
            break;
        case sf::Keyboard::D:
        case sf::Keyboard::Right:
            moving_right = is_pressed;
            break;
        default:
            break;
    }
}

void game::update(
    sf::Time const& dt)
{
    sf::Vector2f movement;

    if(moving_up)
    {
        movement.y -= player_speed;
    }
    if(moving_down)
    {
        movement.y += player_speed;
    }

    if(moving_left)
    {
        movement.x -= player_speed;
    }
    if(moving_right)
    {
        movement.x += player_speed;
    }

    player.move(movement * dt.asSeconds());
}

void game::render()
{
    window.clear();
    window.draw(landscape);
    window.draw(player);
    window.display();
}