#pragma once

#include "player.h"
#include "resources.h"
#include "state/state.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <filesystem>

class application
{
public:
    application();

    void run();

private:
    void process_input();

    void update_statistics(
        sf::Time const& dt);

    void update(
        sf::Time const& dt);

    void render();

    sf::RenderWindow window;
    resources::fonts fonts;
    resources::textures textures;
    player_t player_1;

    state::stack states;

    sf::Text statistics_text;
    sf::Time statistics_update_time;
    std::size_t statistics_num_frames;

    sf::Time const time_per_frame = sf::seconds(1.f / 60.f);
};