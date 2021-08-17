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

    word.setFont(resources::fonts().get(resources::font::main));
    word.setString("Game Paused");	
    word.setCharacterSize(70);
    utility::center_origin(word);
    word.setPosition(0.5f * view_size.x, 0.4f * view_size.y);

    instructions.setFont(resources::fonts().get(resources::font::main));
    instructions.setString("(Press Backspace or Left + Right to return to the main menu)");	
    utility::center_origin(instructions);
    instructions.setPosition(0.5f * view_size.x, 0.6f * view_size.y);

    states.context.music.pause(true);
}

pause::~pause()
{
    states.context.music.pause(false);
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
    // Escape key or Start button pressed, remove itself to return to the game.
    if(event.type == sf::Event::KeyReleased &&
       event.key.code == sf::Keyboard::Escape)
    {
        states.request_pop();
    }
    else if(event.type == sf::Event::JoystickButtonReleased &&
            event.joystickButton.button == 6)
    {
        states.request_pop();
    }

    // Backspace key pressed or Left and Right button pressed, clear stack and push main menu.
    if(event.type == sf::Event::KeyReleased &&
       event.key.code == sf::Keyboard::BackSpace)
    {
        states.request_clear();
        states.request_push(id::menu);
    }
    else if(event.type == sf::Event::JoystickButtonReleased)
    {
        if((event.joystickButton.button == 5 && sf::Joystick::isButtonPressed(0, 7)) ||
           (event.joystickButton.button == 7 && sf::Joystick::isButtonPressed(0, 5)))
        {
            states.request_clear();
            states.request_push(id::menu);
        }
    }


    return false;
}

}
