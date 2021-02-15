#pragma once

#include "gui/button.h"
#include "gui/container.h"
#include "gui/label.h"
#include "player.h"
#include "state/stack.h"
#include "state/state.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <array>
#include <memory>
#include <string>

namespace state
{

class settings : public state
{
public:
    settings(
        stack& states);

    virtual void draw() override;
    [[nodiscard]] virtual bool update(
        sf::Time const& dt) override;
    [[nodiscard]] virtual bool handle_event(
        sf::Event const& event) override;

private:
    void update_labels();

    void add_button_label(
        player_t::action const action,
        float const y,
        std::string const& text);

private:
    sf::Sprite background;
    gui::container container;
    std::array<std::shared_ptr<gui::button>, player_t::action::count> buttons;
    std::array<std::shared_ptr<gui::label>, player_t::action::count> labels;
};

}
