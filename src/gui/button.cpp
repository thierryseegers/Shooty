#include "button.h"

#include "resources.h"
#include "sound.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>

namespace gui
{

void button::state_t::operator=(button::position const p)
{
    switch(p)
    {
    case position::normal:
        sprite.setTextureRect({0, 0, 200, 50});
        break;
    case position::pressed:
        sprite.setTextureRect({0, 100, 200, 50});
        break;
    case position::selected:
        sprite.setTextureRect({0, 50, 200, 50});
        break;
    }
}

button::button(
    state::stack::context_t& context)
    : text{sf::Text{"", resources::fonts().get(resources::font::main), 16}}
    , toggle{false}
    , sound{context.sound}
    , state{sprite}
{
    sprite.setTexture(resources::textures().get(resources::texture::buttons));
    state = position::normal;

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
    state = position::selected;
}

void button::deselect()
{
    component::deselect();
    state = position::normal;
}

void button::activate()
{
    component::activate();

    if(toggle)
    {
        state = position::pressed;
    }

    if(click)
    {
        click();
    }

    if(!toggle)
    {
        deactivate();
    }

    sound.play(resources::sound_effect::button);
}

void button::deactivate()
{
    component::deactivate();

    if(toggle)
    {
        state = component::selected() ? position::selected : position::normal;
    }
}

void button::handle_event(
    const sf::Event&)
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
