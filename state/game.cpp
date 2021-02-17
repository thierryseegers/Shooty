#include "state/game.h"

#include "state/stack.h"

#include <SFML/System.hpp>

namespace state
{

game::game(
    stack& states)
    : state{states}
    , world{states.context.window}
{}

void game::draw()
{
    world.draw();
}

bool game::update(
    sf::Time const& dt)
{
    world.update(dt);
    states.context.player.handle_realtime_input(world.commands());

    return true;
}

bool game::handle_event(
    sf::Event const& event)
{
    // Game input handling.
    states.context.player.handle_event(event, world.commands());

    // Escape pressed, trigger the pause screen.
    if(event.type == sf::Event::KeyPressed &&
       event.key.code == sf::Keyboard::Escape)
    {
        states.request_push(id::pause);
    }

    return true;
}

}