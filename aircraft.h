#pragma once

#include "entity.h"
#include "resources.h"

#include <SFML/Graphics.hpp>

class aircraft : entity
{
public:
    enum class type
    {
        eagle,
        raptor
    };

    explicit aircraft(
        type const type_,
        resources::textures const& textures);

    virtual void draw_self(
        sf::RenderTarget& target,
        sf::RenderStates states) const override;

private:
    type type_;
    sf::Sprite sprite;
};