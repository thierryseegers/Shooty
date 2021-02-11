#pragma once

#include "command.h"

#include <SFML/Window.hpp>

class player_t
{
public:
    void handle_event(
        sf::Event const& event,
        commands_t& commands
        );

    void handle_realtime_input(
        commands_t& commands
    );
};