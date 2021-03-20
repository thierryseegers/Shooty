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
        : text{sf::Text{contents, resources::fonts().get(resources::font::label), 16}}
    {}

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

        void operator=(
            std::string const& contents)
        {
            text_.setString(contents);
        }
    } text;

private:
    virtual void draw(
        sf::RenderTarget& target,
        sf::RenderStates states) const override
    {
        states.transform *= getTransform();
        target.draw(text.text_, states);
    }
};

}
