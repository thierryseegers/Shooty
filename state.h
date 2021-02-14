#pragma once

#include "player.h"
#include "resources.h"
#include "world.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <deque>
#include <functional>
#include <map>
#include <stack>
#include <vector>

namespace state
{
    enum class id
    {
        game,
        loading,
        menu,
        none,
        pause,
        title,
    };
}

class state_t;

class states_t : private sf::NonCopyable
{
public:
    struct context_t
    {
        sf::RenderWindow& window;
        resources::fonts& fonts;
        resources::textures& textures;
        player_t& player;
    } context;

    explicit states_t(
        states_t::context_t context);

    template<class State>
    void register_state(
        state::id const id)
    {
        factory[id] = [this]()
        {
            return std::make_unique<State>(*this);
        };
    }

    void update(
        sf::Time const& dt);
    void draw();
    void handle_event(
        sf::Event const& event);

    void request_push(
        state::id const id);
    void request_pop();
    void request_clear();

    [[nodiscard]] bool empty() const;

private:
    void apply_pending_requests();
    
    std::deque<std::unique_ptr<state_t>> states;
    std::deque<std::function<void ()>> pending_requests;

    std::map<state::id, std::function<std::unique_ptr<state_t> ()>> factory;
};

class state_t
{
public:
    state_t(
        states_t& states)
        : states{states}
    {}

    virtual ~state_t() = default;

    virtual void draw() = 0;
    virtual bool update(sf::Time const& dt) = 0;
    virtual bool handle_event(sf::Event const& event) = 0;

protected:
    states_t& states;
};

namespace states
{

class title : public state_t
{
public:
    title(
        states_t& states);

    virtual void draw() override;
    virtual bool update(
        sf::Time const& dt) override;
    virtual bool handle_event(
        sf::Event const& event) override;

private:
    sf::Sprite background;
    sf::Text text;

    bool show_text;
    sf::Time blink_delay;
};

class menu : public state_t
{
public:
    menu(
        states_t& states);

    virtual void draw() override;
    virtual bool update(
        sf::Time const& dt) override;
    virtual bool handle_event(
        sf::Event const& event) override;

private:
    struct option
    {
        sf::Text text;
        std::function<void ()> activate;
    };

    void update_options();

    sf::Sprite background;
    
    std::vector<option> options;    // First option is the selected option.
};

class game : public state_t
{
public:
    game(
        states_t& states);

    virtual void draw() override;
    virtual bool update(
        sf::Time const& dt) override;
    virtual bool handle_event(
        sf::Event const& event) override;

private:
    world_t world;
};

class pause : public state_t
{
public:
    pause(
        states_t& states);

    virtual void draw() override;
    virtual bool update(
        sf::Time const& dt) override;
    virtual bool handle_event(
        sf::Event const& event) override;

private:
    sf::Text word;
    sf::Text instructions;
};

}