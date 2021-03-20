#include "gui/container.h"

#include "gui/component.h"
#include "utility.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <algorithm>
#include <iterator>
#include <memory>

namespace gui
{

void container::pack(
    std::shared_ptr<component> component)
{
    children.push_back(component);
    if(selected == -1 &&
       component->selectable())
    {
        component->select();
        selected = children.size() - 1;
    }
}

bool container::selectable() const
{
    return false;
}

void container::handle_event(
    sf::Event const& event)
{
    if(selected != -1 &&
       children[selected]->active())
    {
        children[selected]->handle_event(event);
    }
    else if(event.type == sf::Event::KeyReleased)
    {
        if(event.key.code == sf::Keyboard::W ||
           event.key.code == sf::Keyboard::Up)
        {
            select(direction::previous);
        }
        else if(event.key.code == sf::Keyboard::S ||
                event.key.code == sf::Keyboard::Down)
        {
            select(direction::next);
        }
        else if(selected != -1 &&
                (event.key.code == sf::Keyboard::Return ||
                 event.key.code == sf::Keyboard::Space))
        {
            children[selected]->activate();
        }
    }
}

void container::draw(
    sf::RenderTarget& target,
    sf::RenderStates states) const
{
    states.transform *= getTransform();

    for(auto const& child : children)
    {
        target.draw(*child, states);
    }
}

void container::select(
    direction const d)
{
    if(selected == -1)
    {
        return;
    }

    children[selected]->deselect();

    auto s = utility::cyclic_iterator(children, children.begin() + selected);
    if(d == direction::next)
    {
        selected = std::distance(
            children.begin(),
            std::find_if(
                std::next(s),
                s,
                std::mem_fn(&component::selectable)).base());
    }
    else
    {
        selected = std::distance(
            children.begin(),
            std::next(std::find_if(
                std::make_reverse_iterator(s),
                std::make_reverse_iterator(std::next(s)),
                std::mem_fn(&component::selectable))).base().base());
    }

    children[selected]->select();
}

}