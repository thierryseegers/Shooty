#pragma once

#include "action.h"
#include "gui/button.h"
#include "gui/container.h"
#include "gui/label.h"
#include "player.h"
#include "state/stack.h"
#include "state/state.h"

#include <magic_enum.hpp>
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
        action const action,
        float const y,
        std::string const& text);

private:
    sf::Sprite background;
    gui::container container;
    std::array<std::shared_ptr<gui::button>, magic_enum::enum_count<action>()> buttons;
    std::array<std::shared_ptr<gui::label>, magic_enum::enum_count<action>()> key_labels;
    std::array<std::shared_ptr<gui::label>, magic_enum::enum_count<action>()> joy_labels;
};

}
