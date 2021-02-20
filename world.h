#pragma once

#include "command.h"
#include "entity/enemy.h"
#include "entity/leader.h"
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
    void guide_missiles();

    sf::FloatRect view_bounds() const;
    sf::FloatRect battlefield_bounds() const;

    enum layer
    {
        background,
        air,
        count
    };

    sf::RenderWindow& window;
    sf::View view;

    scene::node_t graph;
    scene::layers<layer::count> layers;

    commands_t commands_;

    sf::FloatRect const bounds;
    sf::Vector2f const player_spawn_point;
    float scroll_speed;
    
    entity::leader_t* player;
    std::vector<entity::enemy*> enemies;

    struct spawn
    {
        std::function<std::unique_ptr<entity::aircraft_t> ()> const what;
        sf::Vector2f const where;

        bool operator<(spawn const& other) const
        {
            return where.y < other.where.y;
        }
    };

    std::multiset<spawn> enemy_spawns;
};