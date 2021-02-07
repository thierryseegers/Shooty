#include "scene.h"

#include <SFML/Graphics.hpp>

#include <algorithm>
#include <cassert>
#include <functional>
#include <memory>
#include <numeric>
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

void node::draw_self(
    sf::RenderTarget& target,
    sf::RenderStates states) const
{}

sf::Transform node::world_transform() const
{
    auto transform = sf::Transform::Identity;

    for(scene::node const* n = this; n; n = n->parent)
    {
        transform *= n->getTransform();
    }

    return transform;
}

sf::Vector2f node::world_positiion() const
{
    return world_transform() * sf::Vector2f{};
}

void node::update(
    sf::Time const dt)
{
    update_self(dt);

    for(auto& child : children)
    {
        child->update(dt);
    }
}

void node::update_self(
    sf::Time const dt)
{}

sprite::sprite(
    sf::Texture const& texture)
    : sprite_{texture}
{}

sprite::sprite(
    sf::Texture const& texture,
    sf::IntRect const& rect)
    : sprite_{texture, rect}
{}

void sprite::draw_self(
    sf::RenderTarget& target,
    sf::RenderStates states) const 
{
    target.draw(sprite_, states);
}

}
