#pragma once

#include "entity.h"

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
        type const type_);

    virtual void draw_self(
        sf::RenderTarget& target,
        sf::RenderStates states) const override;

private:
    type type_;
    sf::Sprite sprite;
};