#pragma once

#include "aircraft.h"
#include "resources.h"
#include "scene.h"

#include <SFML/Graphics.hpp>

#include <array>

class world
{
public:
    explicit world(
        sf::RenderWindow& window);

    void update(sf::Time const dt);
    void draw() const;

private:
    void load_textures();
    void build_scene();

    enum layer
    {
        background,
        air,
        count
    };

    sf::RenderWindow& window;
    sf::View view;

    resources::textures textures;
    
    scene::node graph;
    scene::layers<layer::count> layers;

    sf::FloatRect const bounds;
    sf::Vector2f const spawn_position;
    float scroll_speed;
    
    aircraft* player;
};