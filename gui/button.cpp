#include "button.h"

#include "resources.h"

#include <SFML/Graphics.hpp>

namespace gui
{
button::button()
    : normal{utility::single::instance<resources::textures>().get(resources::texture::button_normal)}
    , selected{utility::single::instance<resources::textures>().get(resources::texture::button_selected)}
    , pressed{utility::single::instance<resources::textures>().get(resources::texture::button_pressed)}
    , text{sf::Text{"", utility::single::instance<resources::fonts>().get(resources::font::main), 16}}
    , toggle{false}
{
    sprite.setTexture(normal);

    auto const bounds = sprite.getLocalBounds();
    text.text_.setPosition(bounds.width / 2.f, bounds.height / 2.f);
}

bool button::selectable() const
{
    return true;
}

void button::select()
{
    component::select();
    sprite.setTexture(selected);
}

void button::deselect()
{
    component::deselect();
    sprite.setTexture(normal);
}

void button::activate()
{
    component::activate();

    if(toggle)
    {
        sprite.setTexture(pressed);
    }

    if(click)
    {
        click();
    }

    if(!toggle)
    {
        deactivate();
    }
}

void button::deactivate()
{
    component::deactivate();

    if(toggle)
    {
        sprite.setTexture(component::selected() ? selected : normal);
    }
}

void button::handle_event(
    const sf::Event& event)
{}

void button::draw(
    sf::RenderTarget& target,
    sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(sprite, states);
    target.draw(text.text_, states);
}

}
