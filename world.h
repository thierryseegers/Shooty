#pragma once

#include "command.h"
#include "entity/aircraft.h"
#include "resources.h"
#include "scene.h"

#include <SFML/Graphics.hpp>

#include <array>
#include <set>

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
    void spawn_enemies();

    sf::FloatRect view_bounds() const;
    sf::FloatRect battlefield_bounds() const;

    enum layer
    {
        background,
        air,
        count
    };

    enum class aircraft
    {
        avenger,
        raptor
    };

    sf::RenderWindow& window;
    sf::View view;

    resources::textures textures;
    
    scene::node_t graph;
    scene::layers<layer::count> layers;

    commands_t commands_;

    sf::FloatRect const bounds;
    sf::Vector2f const player_spawn_point;
    float scroll_speed;
    
    entity::aircraft_t* player;

    struct spawn_point
    {
        aircraft const type;
        sf::Vector2f const where;

        bool operator<(spawn_point const& other) const
        {
            return where.y < other.where.y;
        }
    };

    std::multiset<spawn_point> enemy_spawn_points;
};