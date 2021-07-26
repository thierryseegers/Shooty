#pragma once

#include "command.h"

#include <SFML/Window.hpp>

#include <map>

class player_t
{
public:
    enum action
    {
        move_down,
        move_left,
        move_right,
        move_up,
        fire,
        launch_missile,
        count
    };

    enum class mission
    {
        failure,
        running,
        success,
    };

    player_t();

    sf::Keyboard::Key bound_key(
        action const what) const;

    void bind_key(
        sf::Keyboard::Key const key,
        action const what);

    unsigned int bound_joy(
        action const what) const;

    void bind_joy(
        unsigned int const button,
        action const what);

    void handle_event(
        sf::Event const& event,
        commands_t& commands);

    void handle_realtime_input(
        commands_t& commands);

    mission& mission_status()
    {
        return status;
    }

    [[nodiscard]] mission mission_status() const
    {
        return status;
    }

private:
    [[nodiscard]] static bool is_realtime_action(action const a);

    std::map<sf::Keyboard::Key, action> key_bindings;
    std::map<unsigned int, action> joy_bindings;
    std::map<action, command_t> action_bindings;

    mission status;
};