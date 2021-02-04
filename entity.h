#pragma once

#include "scene.h"

#include <SFML/Graphics.hpp>

struct entity : public scene::node
{
    sf::Vector2f velocity;
};
