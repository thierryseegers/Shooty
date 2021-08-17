#pragma once

#include "gui/component.h"
#include "resources.h"
#include "utility.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>

#include <string>

namespace gui
{

class label : public component
{
public:
    label(
        std::string const& contents)
        : text{sf::Text{contents, resources::fonts().get(resources::font::label), 16}, background}
    {
        background.setOrigin(text.text_.getOrigin() + sf::Vector2f{4, 0});
        background.setFillColor(sf::Color::Black * sf::Color{0, 0, 0, 128});
    }

    [[nodiscard]] virtual bool selectable() const override
    {
        return false;
    }

    virtual void handle_event(
        sf::Event const&) override
    {}

    struct
    {
        sf::Text text_;
        sf::RectangleShape& background;

        void operator=(
            std::string const& contents)
        {
            text_.setString(contents);

            sf::FloatRect const text_size = text_.getLocalBounds();
            background.setSize(sf::Vector2f{text_size.width + 8, text_size.height + 8});
        }
    } text;

private:
    virtual void draw(
        sf::RenderTarget& target,
        sf::RenderStates states) const override
    {
        if(!text.text_.getString().isEmpty())
        {
            states.transform *= getTransform();

            target.draw(background, states);
            target.draw(text.text_, states);
        }
    }

    sf::RectangleShape background;
};

}
