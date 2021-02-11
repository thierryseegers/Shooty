#pragma once

#include "entity.h"
#include "scene.h"

#include <SFML/Graphics.hpp>

class aircraft_t : public entity, public scene::sprite_t
{
public:
    enum class type
    {
        eagle,
        raptor
    };

    explicit aircraft_t(
        type const type_,
        sf::Texture const& texture);

protected:
    type type_;
};

struct leader_t : public aircraft_t
{
    using aircraft_t::aircraft_t;
};