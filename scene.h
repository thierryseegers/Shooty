#pragma once

#include "command.h"
#include "resources.h"

#include <SFML/Graphics.hpp>

#include <array>
#include <deque>
#include <memory>
#include <set>
#include <utility>
#include <vector>

namespace scene
{

class node_t : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
{
public:
    class iterator
    {
    public:
        using iterator_category = std::input_iterator_tag;
        using value_type = node_t;
        using reference = node_t&;
        using pointer = node_t*;
        using difference_type = std::ptrdiff_t;

        iterator(
            node_t* root = nullptr);

        reference operator*();

        pointer operator->();

        iterator operator++();

        iterator operator++(int);

        iterator& operator=(iterator const& rhs);

        [[nodiscard]] bool operator!=(iterator const& rhs);

    private:
        node_t* next(node_t* n);

        std::deque<size_t> indices;
        node_t* current;
    };

    virtual ~node_t() = default;

    iterator begin()
    {
        return {this};
    }

    iterator end() const
    {
        return {};
    }

    void attach(
        std::unique_ptr<node_t> child);

    std::unique_ptr<node_t> detach(
        node_t const& node);

    void on_command(
        command_t const& command,
        sf::Time const& dt);

    void update(
        sf::Time const& dt,
        commands_t& commands);

    void sweep_removed();

    virtual void draw(
        sf::RenderTarget& target,
        sf::RenderStates states) const final;

    std::set<std::pair<node_t*, node_t*>> collisions();

    sf::Transform world_transform() const;

    sf::Vector2f world_position() const;

    bool remove = false;

protected:
    friend class const_iterator;
    
    virtual void update_self(
        sf::Time const& dt,
        commands_t& commands);

    virtual void draw_self(
        sf::RenderTarget& target,
        sf::RenderStates states) const;

    [[nodiscard]] bool collides(node_t const* other) const;

    virtual sf::FloatRect collision_bounds() const;

    std::vector<std::unique_ptr<node_t>> children;
    node_t *parent = nullptr;
};

bool collision(
    node_t const& lhs,
    node_t const& rhs);

float distance(
    node_t const& lhs,
    node_t const& rhs);

class background : public node_t
{};

class air : public node_t
{};

template<size_t Count>
using layers = std::array<scene::node_t*, Count>;

class sprite_t : public node_t
{
public:
    explicit sprite_t(
        resources::texture const& texture);

    sprite_t(
        resources::texture const& texture,
        sf::IntRect const& rect);

    virtual ~sprite_t() = default;

protected:
    sf::Sprite sprite;

private:
    virtual void draw_self(
        sf::RenderTarget& target,
        sf::RenderStates states) const override;
};

}