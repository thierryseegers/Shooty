#include "game.h"

#include "resources.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <filesystem>
#include <stdexcept>

game::game()
    : window{sf::VideoMode(640, 480), "Shooty"}
    , world_{window}
    , statistics_num_frames{0}
{
    statistics_font.loadFromFile("Book/03_World/Media/Sansation.ttf");
    statistics_text.setFont(statistics_font);
    statistics_text.setPosition(5.f, 5.f);
    statistics_text.setCharacterSize(16);
}

void game::run()
{
    sf::Clock clock;
    sf::Time elapsed_time = sf::Time::Zero;

     while(window.isOpen())
    {
        elapsed_time += clock.restart();
        while(elapsed_time > time_per_frame)
        {
            elapsed_time -= time_per_frame;
            process_events();
            update(time_per_frame);
        }

        update_statistics(elapsed_time);
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

void game::update_statistics(
    sf::Time const& dt)
{
    statistics_update_time += dt;
    ++statistics_num_frames;

    if(statistics_update_time >= sf::seconds(1.0f))
    {
        statistics_text.setString(
            "Frames / Second = " + std::to_string(statistics_num_frames) + "\n" +
            "Time / Update = " + std::to_string(statistics_update_time.asMicroseconds() / statistics_num_frames) + "us");

        statistics_update_time -= sf::seconds(1.0f);
        statistics_num_frames = 0;
    }
}

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
    window.draw(statistics_text);
    window.display();
}