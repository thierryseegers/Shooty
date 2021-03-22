#include "state/settings.h"

#include "gui/button.h"
#include "gui/container.h"
#include "gui/label.h"
#include "player.h"
#include "resources.h"
#include "state/stack.h"
#include "state/state.h"
#include "utility.h"

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

    // Build key binding buttons and labels.
    add_button_label(player_t::action::move_left,  150.f, "Move Left");
    add_button_label(player_t::action::move_right, 200.f, "Move Right");
    add_button_label(player_t::action::move_up,    250.f, "Move Up");
    add_button_label(player_t::action::move_down,  300.f, "Move Down");
    add_button_label(player_t::action::fire,       350.f, "Fire");
    add_button_label(player_t::action::launch_missile, 400.f, "Launch Missile");

    update_labels();

    // Add back button to go back to main menu.
    auto back = std::make_shared<gui::button>(states.context);
    back->setPosition(80.f, 475.f);
    back->text = "Back";
    back->click = [this]()
    {
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
    
    // Iterate through all key binding buttons to see if they are being pressed, waiting for the user to enter a key.
    for(auto a = 0; a != player_t::action::count && !rebound_key; ++a)
    {
        if(buttons[a]->active())
        {
            rebound_key = true;
            if(event.type == sf::Event::KeyReleased)
            {
                states.context.player.bind_key(event.key.code, static_cast<player_t::action>(a));
                buttons[a]->deactivate();
            }
        }
    }

    // If pressed button changed key bindings, update labels; otherwise consider other buttons in container
    if (rebound_key)
        update_labels();
    else
        container.handle_event(event);

    return false;
}

void settings::update_labels()
{
    for(auto i = 0; i != player_t::action::count; ++i)
    {
        labels[i]->text = utility::to_string(states.context.player.bound_key(static_cast<player_t::action>(i)));
    }
}

void settings::add_button_label(
    player_t::action const action,
    float const y,
    std::string const& text)
{
    buttons[action] = std::make_shared<gui::button>(states.context);
    buttons[action]->setPosition(80.f, y);
    buttons[action]->text = text;
    buttons[action]->toggle = true;

    labels[action] = std::make_shared<gui::label>("");
    labels[action]->setPosition(300.f, y + 15.f);

    container.pack(buttons[action]);
    container.pack(labels[action]);
}

}