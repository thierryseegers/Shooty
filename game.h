#pragma once

#include "resources.h"
#include "world.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class game
{
public:
    game();

    void run();

private:
    void process_events();
    
    void handle_player_input(
        sf::Keyboard::Key const key,
        bool const is_pressed);
    
    void update(
        sf::Time const& dt);
    
    void render();

    sf::RenderWindow window;
    world world_;

    sf::Time const time_per_frame = sf::seconds(1.f / 60.f);
};