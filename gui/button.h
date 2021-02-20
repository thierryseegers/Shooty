#pragma once

#include "component.h"

#include "utility.h"

#include <SFML/Graphics.hpp>

#include <functional>
#include <string>

namespace gui
{

class button : public component
{
public:
    button();

    virtual bool selectable() const override;
    virtual void select() override;
    virtual void deselect() override;

    virtual void activate() override;
    virtual void deactivate() override;

    virtual void handle_event(
        sf::Event const& event) override;

    struct
    {
        sf::Text text_;

        void operator=(
            std::string const& contents)
        {
            text_.setString(contents);
            utility::center_origin(text_);
        }
    } text;

    std::function<void ()> click;
    bool toggle;

private:
    virtual void draw(
        sf::RenderTarget& target,
        sf::RenderStates states) const override;

    sf::Texture const& normal;
    sf::Texture const& selected;
    sf::Texture const& pressed;
    sf::Sprite sprite;
};

}
