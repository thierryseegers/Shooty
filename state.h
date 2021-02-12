#pragma once

#include "player.h"
#include "resources.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <deque>
#include <functional>
#include <map>
#include <stack>

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
        resources::textures& textures;
        resources::fonts& fonts;
        player_t& player;
    } context;

    explicit states_t(
        states_t::context_t& context);

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
    std::stack<std::function<void ()>> pending_requests;

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

private:
    states_t& states;
};


