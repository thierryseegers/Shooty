#pragma once

#include "command.h"
#include "entity/aircraft.h"
#include "resources.h"
#include "scene.h"

#include <SFML/Graphics.hpp>

#include <array>

class world_t
{
public:
    explicit world_t(
        sf::RenderWindow& window);

    void update(sf::Time const dt);
    void draw() const;

    commands_t& commands();

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
    
    scene::node_t graph;
    scene::layers<layer::count> layers;

    commands_t commands_;

    sf::FloatRect const bounds;
    sf::Vector2f const spawn_position;
    float scroll_speed;
    
    entity::aircraft_t* player;
};