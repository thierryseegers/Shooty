#include "state/pause.h"

#include "resources.h"
#include "state/stack.h"
#include "utility.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

namespace state
{

pause::pause(
    stack& states)
    : state{states}
{
    auto const view_size = states.context.window.getView().getSize();

    word.setFont(utility::single::instance<resources::fonts>().get(resources::font::main));
    word.setString("Game Paused");	
    word.setCharacterSize(70);
    utility::center_origin(word);
    word.setPosition(0.5f * view_size.x, 0.4f * view_size.y);

    instructions.setFont(utility::single::instance<resources::fonts>().get(resources::font::main));
    instructions.setString("(Press Backspace to return to the main menu)");	
    utility::center_origin(instructions);
    instructions.setPosition(0.5f * view_size.x, 0.6f * view_size.y);
}

void pause::draw()
{
    auto& window = states.context.window;
    window.setView(window.getDefaultView());

    sf::RectangleShape foreground;
    foreground.setFillColor({0, 0, 0, 150});
    foreground.setSize(sf::Vector2f{window.getSize()});

    window.draw(foreground);
    window.draw(word);
    window.draw(instructions);
}

bool pause::update(
    sf::Time const&)
{
    return false;
}

bool pause::handle_event(
    sf::Event const& event)
{
    if(event.type != sf::Event::KeyPressed)
    {
        return false;
    }

    if(event.key.code == sf::Keyboard::Escape)
    {
        // Escape pressed, remove itself to return to the game.
        states.request_pop();
    }

    if (event.key.code == sf::Keyboard::BackSpace)
    {
        // Escape pressed, remove itself to return to the game.
        states.request_clear();
        states.request_push(id::menu);
    }

    return false;
}

}
