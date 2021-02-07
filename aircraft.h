#pragma once

#include "entity.h"
#include "scene.h"

#include <SFML/Graphics.hpp>

class aircraft : public entity, public scene::sprite
{
public:
    enum class type
    {
        eagle,
        raptor
    };

    explicit aircraft(
        type const type_,
        sf::Texture const& texture);

private:
    type type_;
};