#pragma once

#include "entity.h"
#include "resources.h"

#include <SFML/Graphics/Rect.hpp>

namespace entity
{

class projectile : public entity<>
{
public:
    projectile(
        float const speed,
        int const damage,
    resources::texture const& texture,
    sf::IntRect const& texture_rect);

    virtual ~projectile() = default;

    static float constexpr downward = 1.f;
    static float constexpr upward = -1.f;

    float const speed;
    int const damage;
};

}
