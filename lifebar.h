#pragma once

#include "scene.h"

#include <SFML/Graphics.hpp>

namespace entity
{

class lifebar : public scene::node_t
{
public:
    explicit lifebar(
        float const length,
        int const percent = 100);

    void adjust(
        int const percent);

private:
    virtual void draw_self(
        sf::RenderTarget& target,
        sf::RenderStates states) const override;

    virtual void update_self(
        sf::Time const& dt,
        commands_t& commands) override;

    int percent;
    int target_percent;
    sf::RectangleShape outline, bar;
};

}
