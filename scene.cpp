#include "scene.h"

#include <SFML/Graphics.hpp>

#include <algorithm>
#include <cassert>
#include <memory>
#include <vector>

namespace scene
{

void node::attach(
    std::unique_ptr<node> child)
{
    child->parent = this;
    children.push_back(std::move(child));
}

std::unique_ptr<node> node::detach(
    node const& node_)
{
    auto i = std::find_if(children.begin(), children.end(), [&](auto& p)
        {
            return p.get() == &node_;
        });
    assert(i != children.end());

    auto p = std::move(*i);
    p->parent = nullptr;
    children.erase(i);

    return p;
}

void node::draw(
    sf::RenderTarget& target,
    sf::RenderStates states) const
{
    states.transform *= getTransform();

    draw_self(target, states);

    for(auto& child : children)
    {
        child->draw(target, states);
    }
}

}
