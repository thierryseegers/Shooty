#include "scene.h"

#include "utility.h"

#include <SFML/Graphics.hpp>

#include <algorithm>
#include <cassert>
#include <functional>
#include <iterator>
#include <memory>
#include <numeric>
#include <vector>

namespace scene
{

node_t::iterator::iterator(
    node_t* root)
{
    if(root)
    {
        indices.push_back(0);
        current = next(root);
    }
    else
    {
        current = nullptr;
    }
}

node_t::iterator::reference node_t::iterator::operator*()
{
    return *current;
}

node_t::iterator::pointer node_t::iterator::operator->()
{
    return current;
}

node_t::iterator node_t::iterator::operator++()
{
    if(current->parent && ++indices.back() != current->parent->children.size())
    {
        current = next(current->parent->children[indices.back()].get());
    }
    else
    {
        current = current->parent;
        if(current)
        {
            indices.pop_back();
        }
    }

    return *this;
}

node_t::iterator node_t::iterator::operator++(int)
{
    auto tmp = *this;
    this->operator++();
    return tmp;
}

node_t::iterator& node_t::iterator::operator=(iterator const& rhs)
{
    current = rhs.current;
    indices = rhs.indices;

    return *this;
}

bool node_t::iterator::operator!=(iterator const& rhs)
{
    return rhs.current != current;
}

node_t* node_t::iterator::next(node_t* n)
{
    if(n && n->children.size())
    {
        indices.push_back(0);
        return next(n->children[0].get());
    }
    else
        return n;
}

void node_t::attach(
    std::unique_ptr<node_t> child)
{
    child->parent = this;
    children.push_back(std::move(child));
}

void node_t::on_command(
    command_t const& command,
    sf::Time const& dt)
{
    command(*this, dt);

    for(auto& child : children)
    {
        child->on_command(command, dt);
    }
}

void node_t::update(
    sf::Time const& dt,
    commands_t& commands)
{
    update_self(dt, commands);

    for(auto& child : children)
    {
        child->update(dt, commands);
    }
}

void node_t::sweep_removed()
{
    children.erase(std::remove_if(children.begin(), children.end(), [](auto const& n){ return n->remove; }), children.end());
    std::for_each(children.begin(), children.end(), std::mem_fn(&node_t::sweep_removed));
}

void node_t::draw(
    sf::RenderTarget& target,
    sf::RenderStates states) const
{
    states.transform *= getTransform();

    draw_self(target, states);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::C))
    {
        sf::FloatRect const rect = collision_bounds();

        sf::RectangleShape shape;
        shape.setPosition(sf::Vector2f(rect.left, rect.top));
        shape.setSize(sf::Vector2f(rect.width, rect.height));
        shape.setFillColor(sf::Color::Transparent);
        shape.setOutlineColor(sf::Color::Green);
        shape.setOutlineThickness(1.f);

        target.draw(shape);
    }

    for(auto& child : children)
    {
        child->draw(target, states);
    }
}

sf::Transform node_t::world_transform() const
{
    auto transform = sf::Transform::Identity;

    for(scene::node_t const* n = this; n; n = n->parent)
    {
        transform = n->getTransform() * transform;
    }

    return transform;
}

sf::Vector2f node_t::world_position() const
{
    return world_transform() * sf::Vector2f{};
}

std::set<std::pair<node_t*, node_t*>> node_t::collisions()
{
    std::set<std::pair<node_t*, node_t*>> c;

    for(auto left = begin(), e = end(); left != e; ++left)
    {
        for(auto right = std::next(left); right != e; ++right)
        {
            if(left->collides(&(*right)))
            {
                c.insert(std::minmax(&(*left), &(*right)));
            }
        }
    }

    return c;
}

void node_t::draw_self(
    sf::RenderTarget& target,
    sf::RenderStates states) const
{}

void node_t::update_self(
    sf::Time const& dt,
    commands_t& commands)
{}

bool node_t::collides(node_t const* other) const
{
    return collision_bounds().intersects(other->collision_bounds());
}

sf::FloatRect node_t::collision_bounds() const
{
    return {};
}

float distance(
    node_t const& lhs,
    node_t const& rhs)
{
    return utility::length(lhs.world_position() - rhs.world_position());
}

sprite_t::sprite_t(
    resources::texture const& texture)
    : sprite{resources::textures().get(texture)}
{}

sprite_t::sprite_t(
    resources::texture const& texture,
    sf::IntRect const& rect)
    : sprite{resources::textures().get(texture), rect}
{}

void sprite_t::draw_self(
    sf::RenderTarget& target,
    sf::RenderStates states) const 
{
    target.draw(sprite, states);
}

}
