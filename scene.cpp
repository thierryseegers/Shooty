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

void node_t::attach(
    std::unique_ptr<node_t> child)
{
    child->parent = this;
    children.push_back(std::move(child));
}

std::unique_ptr<node_t> node_t::detach(
    node_t const& node)
{
    auto i = std::find_if(children.begin(), children.end(), [&](auto& p)
        {
            return p.get() == &node;
        });
    assert(i != children.end());

    auto p = std::move(*i);
    p->parent = nullptr;
    children.erase(i);

    return p;
}

void node_t::draw(
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

void node_t::draw_self(
    sf::RenderTarget& target,
    sf::RenderStates states) const
{}

sf::Transform node_t::world_transform() const
{
    auto transform = sf::Transform::Identity;

    for(scene::node_t const* n = this; n; n = n->parent)
    {
        transform *= n->getTransform();
    }

    return transform;
}

sf::Vector2f node_t::world_positiion() const
{
    return world_transform() * sf::Vector2f{};
}

void node_t::update(
    sf::Time const dt)
{
    update_self(dt);

    for(auto& child : children)
    {
        child->update(dt);
    }
}

void node_t::on_command(
    command_t const& command,
    sf::Time const dt)
{
    command(*this, dt);

    for(auto& child : children)
    {
        child->on_command(command, dt);
    }
}

void node_t::update_self(
    sf::Time const dt)
{}

sprite_t::sprite_t(
    sf::Texture const& texture)
    : sprite{texture}
{}

sprite_t::sprite_t(
    sf::Texture const& texture,
    sf::IntRect const& rect)
    : sprite{texture, rect}
{}

void sprite_t::draw_self(
    sf::RenderTarget& target,
    sf::RenderStates states) const 
{
    target.draw(sprite, states);
}

}
