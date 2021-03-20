#pragma once

#include "entity/animation.h"
#include "entity/entity.h"
#include "resources.h"

#include <SFML/System/Vector2.hpp>

namespace entity
{

class explosion : public animation
{
public:
    explosion(
        sf::Vector2f const position)
        : animation{resources::texture::explosion, sf::Vector2i{256, 256}, 16, sf::seconds(1), false, position}
    {}

    virtual ~explosion() = default;
};

}
