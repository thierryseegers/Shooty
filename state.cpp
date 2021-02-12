#include "state.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <deque>
#include <functional>
#include <map>
#include <memory>
#include <stack>

states_t::states_t(
    states_t::context_t& context)
    : context{context}
{}

void states_t::update(sf::Time const& dt)
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

void states_t::handle_event(sf::Event const& event)
{
    for(auto i = states.rbegin(); i != states.rend() && (*i)->handle_event(event); ++i)
    {}

    apply_pending_requests();
}

void states_t::request_push(state::id const id)
{
    pending_requests.push([=]()
    {
        states.push_back(factory[id]());
    });
}

void states_t::request_pop()
{
    pending_requests.push([=]()
    {
        states.pop_back();
    });
}

void states_t::request_clear()
{
    pending_requests.push([=]()
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
        pending_requests.top()();
        pending_requests.pop();
    }
}

