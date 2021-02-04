#pragma once

#include <SFML/Graphics.hpp>

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
    virtual void draw_self(
        sf::RenderTarget& target,
        sf::RenderStates states) const = 0;

private:
    std::vector<std::unique_ptr<node>> children;
    node *parent;
};

}