#include "application.h"

#include "resources.h"
#include "state/game.h"
#include "state/id.h"
#include "state/menu.h"
#include "state/pause.h"
#include "state/title.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <filesystem>
#include <stdexcept>

application::application()
    : window{sf::VideoMode(640, 480), "Shooty", sf::Style::Close}
    , states{{window, fonts, textures, player_1}}
    , statistics_num_frames{0}
{
    window.setKeyRepeatEnabled(false);

    fonts.load(resources::font::main, "Book/06_Menus/Media/Sansation.ttf");
    textures.load(resources::texture::title_screen, "Book/06_Menus/Media/Textures/TitleScreen.png");
	textures.load(resources::texture::button_normal, "Book/06_Menus/Media/Textures/ButtonNormal.png");
	textures.load(resources::texture::button_selected, "Book/06_Menus/Media/Textures/ButtonSelected.png");
	textures.load(resources::texture::button_pressed, "Book/06_Menus/Media/Textures/ButtonPressed.png");

    statistics_text.setFont(fonts.get(resources::font::main));
    statistics_text.setPosition(5.f, 5.f);
    statistics_text.setCharacterSize(24);

    states.register_state<state::title>(state::id::title);
	states.register_state<state::menu>(state::id::menu);
	states.register_state<state::game>(state::id::game);
	states.register_state<state::pause>(state::id::pause);

    states.request_push(state::id::title);
}

void application::run()
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

            // Check inside this loop, because stack might be empty before update() call
            if(states.empty())
            {
                window.close();
            }
        }

        update_statistics(elapsed_time);
        render();
    }
}

void application::process_input()
{
    sf::Event event;
    while(window.pollEvent(event))
    {
        states.handle_event(event);

        if(event.type == sf::Event::Closed)
        {
            window.close();
        }
    }
}

void application::update_statistics(
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

void application::update(
    sf::Time const& dt)
{
    states.update(dt);
}

void application::render()
{
    window.clear();
    states.draw();

    window.setView(window.getDefaultView());
    window.draw(statistics_text);
    window.display();
}