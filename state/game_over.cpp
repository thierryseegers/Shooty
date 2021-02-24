#include "game_over.h"

#include "state/stack.h"
#include "state/state.h"
#include "resources.h"
#include "utility.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

namespace state
{

game_over::game_over(
    stack& states)
    : state{states}
{
    sf::Font const& font = utility::single::instance<resources::fonts>().get(resources::font::main);
    text.setFont(font);

    if(states.context.player.mission_status() == player_t::mission::failure)
        text.setString("Mission failed!");	
    else
        text.setString("Mission successful!");

    text.setCharacterSize(70);
    utility::center_origin(text);

    sf::Vector2f const window_size(states.context.window.getSize());
    text.setPosition(0.5f * window_size.x, 0.4f * window_size.y);
}

void game_over::draw()
{
    sf::RenderWindow& window = states.context.window;
    window.setView(window.getDefaultView());

    // Create dark, semi-transparent background.
    sf::RectangleShape background;
    background.setFillColor(sf::Color(0, 0, 0, 150));
    background.setSize(window.getView().getSize());

    window.draw(background);
    window.draw(text);
}

bool game_over::update(
    sf::Time const& dt)
{
    // Show state for 3 seconds, after return to menu
    elapsed += dt;
    if(elapsed > sf::seconds(3))
    {
        states.request_clear();
        states.request_push(id::menu);
    }

    return false;
}

bool game_over::handle_event(
    sf::Event const& event)
{
    return false;
}

}
