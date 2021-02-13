#include "state.h"
#include "utility.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <algorithm>
#include <deque>
#include <functional>
#include <map>
#include <memory>
#include <stack>
#include <vector>

#include <iostream>

states_t::states_t(
    states_t::context_t context)
    : context{context}
{}

void states_t::update(
    sf::Time const& dt)
{
    for (auto i = states.rbegin(); i != states.rend() && (*i)->update(dt); ++i)
    {}

    apply_pending_requests();
}

void states_t::draw()
{
    for(auto& state : states)
    {
        state->draw();
    }
}

void states_t::handle_event(
    sf::Event const& event)
{
    for(auto i = states.rbegin(); i != states.rend() && (*i)->handle_event(event); ++i)
    {}

    apply_pending_requests();
}

void states_t::request_push(
    state::id const id)
{
    pending_requests.push_back([=]()
    {
        states.push_back(factory[id]());
    });
}

void states_t::request_pop()
{
    pending_requests.push_back([=]()
    {
        states.pop_back();
    });
}

void states_t::request_clear()
{
    pending_requests.push_back([=]()
    {
        states.clear();
    });
}

bool states_t::empty() const
{
    return states.empty();
}

void states_t::apply_pending_requests()
{
    while(!pending_requests.empty())
    {
        pending_requests.front()();
        pending_requests.pop_front();
    }
}

namespace states
{

title::title(
    states_t& states)
    : state_t(states)
    , show_text(true)
    , blink_delay(sf::Time::Zero)
{
    background.setTexture(states.context.textures.get(resources::texture::title_screen));

    text.setFont(states.context.fonts.get(resources::font::main));
    text.setString("Press any key to start");
    center_origin(text);
    text.setPosition(states.context.window.getView().getSize() / 2.f);
}

void title::draw()
{
    auto& window = states.context.window;
    window.draw(background);

    if(show_text)
    {
        window.draw(text);
    }
}

bool title::update(
    sf::Time const& dt)
{
    blink_delay += dt;

    if(blink_delay >= sf::seconds(0.5f))
    {
        show_text = !show_text;
        blink_delay = sf::Time::Zero;
    }

    return true;
}

bool title::handle_event(
    sf::Event const& event)
{
    if(event.type == sf::Event::KeyPressed)
    {
        states.request_pop();
        states.request_push(state::id::menu);
    }

    return true;
}

menu::menu(
    states_t& states)
    : state_t{states}
{
    background.setTexture(states.context.textures.get(resources::texture::title_screen));

    sf::Text play_text;
    play_text.setFont(states.context.fonts.get(resources::font::main));
    play_text.setString("Play");
    center_origin(play_text);
    play_text.setPosition(states.context.window.getView().getSize() / 2.f);

    sf::Text exit_text;
    exit_text.setFont(states.context.fonts.get(resources::font::main));
    exit_text.setString("Exit");
    center_origin(exit_text);
    exit_text.setPosition(play_text.getPosition() + sf::Vector2f{0.f, 30.f});

    options.push_back(option{play_text, [&]()
        {
            states.request_pop();
            states.request_push(state::id::game);
        }});
    options.push_back(option{exit_text, [&]()
        {
            states.request_clear();
        }});

    update_options();
}

void menu::draw()
{
    auto& window = states.context.window;
    window.draw(background);

    for(auto const& option : options)
    {
        window.draw(option.text);
    }
}

bool menu::update(
    sf::Time const&)
{
    return true;
}

bool menu::handle_event(
    sf::Event const& event)
{
    if(event.type != sf::Event::KeyPressed)
    {
        return false;
    }

    switch(event.key.code)
    {
    case sf::Keyboard::Return:
        options.begin()->activate();
        break;
    case sf::Keyboard::Up:
        std::rotate(options.rbegin(), options.rbegin() + 1, options.rend());
        update_options();
        break;
    case sf::Keyboard::Down:
        std::rotate(options.begin(), options.begin() + 1, options.end());
        update_options();
        break;
    default:
        break;
    }

    return true;
}


void menu::update_options()
{
    if(options.empty())
    {
        return;
    }

    // Make all options white and the selected option red.
    for(auto& option : options)
    {
        option.text.setFillColor(sf::Color::White);
    }

    options.begin()->text.setFillColor(sf::Color::Red);
}

game::game(
    states_t& states)
    : state_t{states}
    , world{states.context.window}
{}

void game::draw()
{
    world.draw();
}

bool game::update(
    sf::Time const& dt)
{
    world.update(dt);
    states.context.player.handle_realtime_input(world.commands());

    return true;
}

bool game::handle_event(
    sf::Event const& event)
{
    // Game input handling.
    states.context.player.handle_event(event, world.commands());

    // Escape pressed, trigger the pause screen.
    if(event.type == sf::Event::KeyPressed &&
       event.key.code == sf::Keyboard::Escape)
    {
        states.request_push(state::id::pause);
    }

    return true;
}

pause::pause(
    states_t& states)
    : state_t{states}
{
    auto const view_size = states.context.window.getView().getSize();

    word.setFont(states.context.fonts.get(resources::font::main));
    word.setString("Game Paused");	
    word.setCharacterSize(70);
    center_origin(word);
    word.setPosition(0.5f * view_size.x, 0.4f * view_size.y);

    instructions.setFont(states.context.fonts.get(resources::font::main));
    instructions.setString("(Press Backspace to return to the main menu)");	
    center_origin(instructions);
    instructions.setPosition(0.5f * view_size.x, 0.6f * view_size.y);
}

void pause::draw()
{
    auto& window = states.context.window;
    window.setView(window.getDefaultView());

    sf::RectangleShape foreground;
    foreground.setFillColor({0, 0, 0, 150});
    foreground.setSize(sf::Vector2f{window.getSize()});

    window.draw(foreground);
    window.draw(word);
    window.draw(instructions);
}

bool pause::update(
    sf::Time const&)
{
    return false;
}

bool pause::handle_event(
    sf::Event const& event)
{
    if(event.type != sf::Event::KeyPressed)
    {
        return false;
    }

    if(event.key.code == sf::Keyboard::Escape)
    {
        // Escape pressed, remove itself to return to the game.
        states.request_pop();
    }

    if (event.key.code == sf::Keyboard::BackSpace)
    {
        // Escape pressed, remove itself to return to the game.
        states.request_clear();
        states.request_push(state::id::menu);
    }

    return false;
}

}