#pragma once

#include "entity/entity.h"
#include "lifebar.h"
#include "scene.h"

#include <SFML/Graphics.hpp>

namespace entity
{

class aircraft_t : public entity, public scene::sprite_t
{
public:

    explicit aircraft_t(
        int const starting_life,
        sf::Texture const& texture);

    virtual void update_self(
        sf::Time const& dt) override;

private:
    lifebar *bar;
};

}
