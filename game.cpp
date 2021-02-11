#include "game.h"

#include "resources.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <filesystem>
#include <stdexcept>

game::game()
    : window{sf::VideoMode(640, 480), "Shooty", sf::Style::Close}
    , world{window}
    , statistics_num_frames{0}
{
    window.setKeyRepeatEnabled(false);

    statistics_font.loadFromFile("Book/04_Input/Media/Sansation.ttf");
    statistics_text.setFont(statistics_font);
    statistics_text.setPosition(5.f, 5.f);
    statistics_text.setCharacterSize(24);
}

void game::run()
{
    sf::Clock clock;
    sf::Time last_update = sf::Time::Zero;

     while(window.isOpen())
    {
        sf::Time const elapsed_time = clock.restart();
        last_update += elapsed_time;
        while(last_update > time_per_frame)
        {
            last_update -= time_per_frame;
            process_input();
            update(time_per_frame);
        }

        update_statistics(elapsed_time);
        render();
    }
}

void game::process_input()
{
    sf::Event event;
    while(window.pollEvent(event))
    {
        player_1.handle_event(event, world.commands());

        if(event.type == sf::Event::Closed)
        {
            window.close();
        }
    }

    player_1.handle_realtime_input(world.commands());
}

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
    world.update(dt);
}

void game::render()
{
    window.clear();
    world.draw();
    
    window.setView(window.getDefaultView());
    window.draw(statistics_text);
    window.display();
}