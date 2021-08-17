#include "state/settings.h"

#include "action.h"
#include "bindings.h"
#include "gui/button.h"
#include "gui/container.h"
#include "gui/label.h"
#include "player.h"
#include "resources.h"
#include "state/stack.h"
#include "state/state.h"
#include "tomlpp.h"
#include "utility.h"

#include <magic_enum.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <array>
#include <memory>
#include <string>

namespace state
{

settings::settings(
    stack& states)
    : state{states}
{
    background.setTexture(resources::textures().get(resources::texture::title_screen));

    // Build key binding buttons, key_labels and joy_labels.
    add_button_label(action::move_left,  150.f, "Move Left");
    add_button_label(action::move_right, 200.f, "Move Right");
    add_button_label(action::move_up,    250.f, "Move Up");
    add_button_label(action::move_down,  300.f, "Move Down");
    add_button_label(action::fire,       350.f, "Fire");
    add_button_label(action::launch_missile, 400.f, "Launch Missile");

    update_labels();

    // Add back button to go back to main menu.
    auto back = std::make_shared<gui::button>(states.context);
    back->setPosition(80.f, 475.f);
    back->text = "Back";
    back->click = [this]()
    {
        bindings::save();   // Save the bindings to file.
        state::states.request_pop();
    };

    container.pack(back);
}

void settings::draw()
{
    auto& window = states.context.window;

    window.draw(background);
    window.draw(container);
}

bool settings::update(
    sf::Time const&)
{
    return true;
}

bool settings::handle_event(
    sf::Event const& event)
{
    bool rebound_key = false;
    
    // Iterate through all key binding buttons to see if they are being pressed, waiting for the user to enter a key or a joystick button.
    for(auto i = 0; i != magic_enum::enum_count<action>() && !rebound_key; ++i)
    {
        if(buttons[i]->active())
        {
            rebound_key = true;
            if(event.type == sf::Event::KeyReleased)
            {
                bindings::bind_key(event.key.code, magic_enum::enum_cast<action>(i).value());
                buttons[i]->deactivate();
            }
            else if(event.type == sf::Event::JoystickButtonReleased)
            {
                bindings::bind_button(event.joystickButton.button, magic_enum::enum_cast<action>(i).value());
                buttons[i]->deactivate();
            }
        }
    }

    // If pressed button changed key bindings, update key_labels and joy_labels; otherwise consider other buttons in container
    if(rebound_key)
        update_labels();
    else
        container.handle_event(event);

    return false;
}

void settings::update_labels()
{
    for(auto const a : magic_enum::enum_values<action>())
    {
        key_labels[magic_enum::enum_integer(a)]->text = utility::to_string(bindings::bound_key(a));
    }

    for(auto const a : {action::fire, action::launch_missile})
    {
        joy_labels[magic_enum::enum_integer(a)]->text = "Button " + std::to_string(bindings::bound_button(a));
        joy_labels[magic_enum::enum_integer(a)]->text = "Button " + std::to_string(bindings::bound_button(a));
    }
}

void settings::add_button_label(
    action const a,
    float const y,
    std::string const& text)
{
    std::size_t const i = magic_enum::enum_integer(a);

    buttons[i] = std::make_shared<gui::button>(states.context);
    buttons[i]->setPosition(80.f, y);
    buttons[i]->text = text;
    buttons[i]->toggle = true;

    key_labels[i] = std::make_shared<gui::label>("");
    key_labels[i]->setPosition(300.f, y + 15.f);

    joy_labels[i] = std::make_shared<gui::label>("");
    joy_labels[i]->setPosition(400.f, y + 15.f);

    container.pack(buttons[i]);
    container.pack(key_labels[i]);
    container.pack(joy_labels[i]);
}

}