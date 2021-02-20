#pragma once

#include "command.h"

#include <SFML/Graphics.hpp>

#include <array>
#include <memory>
#include <vector>

namespace scene
{

class node_t : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
{
public:
    virtual ~node_t() = default;

    void attach(
        std::unique_ptr<node_t> child);
    std::unique_ptr<node_t> detach(
        node_t const& node);

    virtual void draw(
        sf::RenderTarget& target,
        sf::RenderStates states) const final;

    sf::Transform world_transform() const;
    sf::Vector2f world_position() const;

    void update(
        sf::Time const& dt,
        commands_t& commands);

    void on_command(
        command_t const& command,
        sf::Time const& dt);

protected:
    virtual void draw_self(
        sf::RenderTarget& target,
        sf::RenderStates states) const;

    virtual void update_self(
        sf::Time const& dt,
        commands_t& commands);

    std::vector<std::unique_ptr<node_t>> children;
    node_t *parent = nullptr;
};

float distance(
    node_t const& lhs,
    node_t const& rhs);

class background : public node_t
{};

class air : public node_t
{};

template<size_t Count>
using layers = std::array<scene::node_t*, Count>;

class sprite_t : public virtual node_t
{
public:
    explicit sprite_t(
        sf::Texture const& texture);
    sprite_t(
        sf::Texture const& texture,
        sf::IntRect const& rect);

protected:
    sf::Sprite sprite;

private:
    virtual void draw_self(
        sf::RenderTarget& target,
        sf::RenderStates states) const override;
};

}