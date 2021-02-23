#pragma once

#include "entity.h"

#include <SFML/Graphics.hpp>

namespace entity
{

class projectile : public entity
{
public:
    projectile(
        float const speed,
        int const damage,
        sf::Texture const& texture);

    virtual ~projectile() = default;

    static float constexpr downward = 1.f;
    static float constexpr upward = -1.f;

    float const speed;
    int const damage;
};

}
