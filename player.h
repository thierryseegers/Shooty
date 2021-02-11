#pragma once

#include "command.h"

#include <SFML/Window.hpp>

#include <map>

class player_t
{
public:
    enum class action
    {
        move_down,
        move_left,
        move_right,
        move_up,
        print_position
    };

    player_t();

    sf::Keyboard::Key bound_key(
        action const what) const;

    void bind_key(
        sf::Keyboard::Key const key,
        action const what);

    void handle_event(
        sf::Event const& event,
        commands_t& commands);

    void handle_realtime_input(
        commands_t& commands);

private:
    [[nodiscard]] static bool is_realtime_action(action const a);

    std::map<sf::Keyboard::Key, action> key_bindings;
    std::map<action, command_t> action_bindings;
};