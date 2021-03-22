#include "state/menu.h"

#include "gui/button.h"
#include "music.h"
#include "resources.h"
#include "state/stack.h"
#include "utility.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <memory>

namespace state
{

menu::menu(
    stack& states)
    : state{states}
{
    background.setTexture(resources::textures().get(resources::texture::title_screen));

    auto play = std::make_shared<gui::button>(states.context);
    play->setPosition(100, 250);
    play->text = "Play";
    play->click = [this]()
    {
        state::states.request_pop();
        state::states.request_push(id::game);
    };
    container.pack(play);

    auto settings = std::make_shared<gui::button>(states.context);
    settings->setPosition(100, 300);
    settings->text = "Settings";
    settings->click = [this]()
    {
        state::states.request_push(id::settings);
    };
    container.pack(settings);

    auto exit = std::make_shared<gui::button>(states.context);
    exit->setPosition(100, 350);
    exit->text = "Exit";
    exit->click = [this]()
    {
        state::states.request_clear();
    };
    container.pack(exit);

    states.context.music.play(music::theme::menu);
}

void menu::draw()
{
    auto& window = states.context.window;

    window.setView(window.getDefaultView());

    window.draw(background);
    window.draw(container);
}

bool menu::update(
    sf::Time const&)
{
    return true;
}

bool menu::handle_event(
    sf::Event const& event)
{
    container.handle_event(event);
    return false;
}

}
