#include "state/title.h"

#include "resources.h"
#include "state/stack.h"
#include "utility.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

namespace state
{

title::title(
    stack& states)
    : state(states)
    , show_text(true)
    , blink_delay(sf::Time::Zero)
{
    background.setTexture(states.context.textures.get(resources::texture::title_screen));

    text.setFont(states.context.fonts.get(resources::font::main));
    text.setString("Press any key to start");
    utility::center_origin(text);
    text.setPosition(states.context.window.getView().getSize() / 2.f);
}

void title::draw()
{
    auto& window = states.context.window;
    window.draw(background);

    if(show_text)
    {
        window.draw(text);
    }
}

bool title::update(
    sf::Time const& dt)
{
    blink_delay += dt;

    if(blink_delay >= sf::seconds(0.5f))
    {
        show_text = !show_text;
        blink_delay = sf::Time::Zero;
    }

    return true;
}

bool title::handle_event(
    sf::Event const& event)
{
    if(event.type == sf::Event::KeyPressed)
    {
        states.request_pop();
        states.request_push(id::menu);
    }

    return true;
}

}
