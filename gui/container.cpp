#include "gui/container.h"

#include "gui/component.h"
#include "utility.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <algorithm>
#include <memory>

namespace gui
{

void container::pack(
    std::shared_ptr<component> component)
{
    children.push_back(component);
    if(selected == children.end() &&
       component->selectable())
    {
        component->select();
        selected = children.rbegin().base();
    }
}

bool container::selectable() const
{
    return false;
}

void container::handle_event(
    sf::Event const& event)
{
    if(selected != children.end() &&
       (*selected)->active())
    {
        (*selected)->handle_event(event);
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
            select(direction::previous);
        }
        else if(event.key.code == sf::Keyboard::Return ||
                event.key.code == sf::Keyboard::Space)
        {
            (*selected)->activate();
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
    if(selected != children.end())
    {
        return;
    }

    (*selected)->deselect();

    auto s = utility::cyclical_iterator(children, selected);
    if(d == direction::next)
    {
        selected = std::find_if(
            std::next(s),
            s,
            std::mem_fn(&component::selectable)).base();
    }
    else
    {
        selected = std::find_if(
            std::make_reverse_iterator(std::prev(s)),
            std::make_reverse_iterator(s),
            std::mem_fn(&component::selectable)).base().base();
    }

    (*selected)->select();
}

}