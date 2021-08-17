#pragma once

#include "gui/container.h"
#include "state/stack.h"
#include "state/state.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

namespace state
{

class menu : public state
{
public:
    menu(
        stack& states);

    virtual void draw() override;
    virtual bool update(
        sf::Time const& dt) override;
    virtual bool handle_event(
        sf::Event const& event) override;

private:
    sf::Sprite background;
    gui::container container;
};

}