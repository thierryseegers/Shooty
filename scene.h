#pragma once

#include <SFML/Graphics.hpp>

#include <array>
#include <memory>
#include <vector>

namespace scene
{

class node : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
{
public:
    void attach(
        std::unique_ptr<node> child);
    std::unique_ptr<node> detach(
        node const& node_);

    virtual void draw(
        sf::RenderTarget& target,
        sf::RenderStates states) const final;

    sf::Transform world_transform() const;
    sf::Vector2f world_positiion() const;

    void update(
        sf::Time const dt);

private:
    virtual void draw_self(
        sf::RenderTarget& target,
        sf::RenderStates states) const;

    virtual void update_self(
        sf::Time const dt);

    std::vector<std::unique_ptr<node>> children;
    node *parent;
};

template<size_t Count>
using layers = std::array<scene::node*, Count>;

class sprite : public node
{
public:
    explicit sprite(
        sf::Texture const& texture);
    sprite(
        sf::Texture const& texture,
        sf::IntRect const& rect);

private:
    virtual void draw_self(
        sf::RenderTarget& target,
        sf::RenderStates states) const override;
    
    sf::Sprite sprite_;
};

}