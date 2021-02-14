#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

namespace gui
{

class component
    : public sf::Drawable
    , public sf::Transformable
    , private sf::NonCopyable
{
public:
    virtual ~component() = default;

    virtual bool selectable() const = 0;
    
    [[nodiscard]] bool selected() const
    {
        return selected_;
    }

    virtual void select()
    {
        selected_ = true;
    }

    virtual void deselect()
    {
        selected_ = false;
    }

    [[nodiscard]] virtual bool active() const
    {
        return active_;
    }

    virtual void activate()
    {
        active_ = true;
    }

    virtual void deactivate()
    {
        active_ = false;
    }

    virtual void handle_event(
        sf::Event const& event) = 0;

private:
    bool selected_ = false;
    bool active_ = false;
};

}