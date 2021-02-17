#include "world.h"

#include "configuration.h"
#include "entity/enemy.h"
#include "entity/leader.h"
#include "resources.h"
#include "scene.h"

#include <SFML/Graphics.hpp>

#include <array>
#include <cmath>
#include <memory>

world_t::world_t(
    sf::RenderWindow& window)
    : window{window}
    , view{window.getDefaultView()}
    , bounds{0.f, 0.f, view.getSize().x, 2000.f}
    , player_spawn_point{view.getSize().x / 2.f, bounds.height - view.getSize().y / 2.f}
    , scroll_speed{-50.f}
    , player{nullptr}
{
    load_textures();
    build_scene();

    view.setCenter(player_spawn_point);
}

void world_t::load_textures()
{
    textures.load(resources::texture::desert, "Media/Textures/Desert.png");
    textures.load(resources::texture::avenger, *configuration::instance()["avenger"]["texture"].value<std::string>());
    textures.load(resources::texture::eagle, *configuration::instance()["leader"]["texture"].value<std::string>());
    textures.load(resources::texture::raptor, *configuration::instance()["raptor"]["texture"].value<std::string>());
}

void world_t::build_scene()
{
    // Create layers.
    for(std::size_t i = 0; i != layer::count; ++i)
    {
        auto layer = std::make_unique<scene::node_t>();
        layers[i] = layer.get();

        graph.attach(std::move(layer));
    }

    // Create background sprite and move to background layer.
    sf::Texture& background_texture = textures.get(resources::texture::desert);
    background_texture.setRepeated(true);
    sf::IntRect background_rect{bounds};

    auto background_sprite = std::make_unique<scene::sprite_t>(background_texture, background_rect);
    background_sprite->setPosition(bounds.left, bounds.top);
    layers[layer::background]->attach(std::move(background_sprite));

    // Create leader aircraft and move to air layer.
    auto leader = std::make_unique<entity::leader_t>(textures);
    player = leader.get();
    player->scene::sprite_t::setPosition(player_spawn_point);
    layers[layer::air]->attach(std::move(leader));

    auto add_enemy = [=](aircraft const a, sf::Vector2f const ds) mutable
        {
            enemy_spawn_points.insert({a, player_spawn_point + ds});
        };
    add_enemy(aircraft::raptor,     {+0.,   -500.});
    add_enemy(aircraft::raptor,     {+0.,   -1000.});
    add_enemy(aircraft::raptor,     {+100., -1100.});
    add_enemy(aircraft::raptor,     {-100., -1100.});
    add_enemy(aircraft::avenger,    {-70.,  -1400.});
    add_enemy(aircraft::avenger,    {-70.,  -1600.});
    add_enemy(aircraft::avenger,    {+70.,  -1400.});
    add_enemy(aircraft::avenger,    {+70.,  -1600.});
}

void world_t::spawn_enemies()
{
    while(!enemy_spawn_points.empty() &&
          enemy_spawn_points.rbegin()->where.y > battlefield_bounds().top)
    {
        auto const enemy_spawn_point = enemy_spawn_points.extract(*enemy_spawn_points.rbegin()).value();

        std::unique_ptr<entity::enemy> e;
        switch(enemy_spawn_point.type)
        {
        case aircraft::avenger:
            e = std::make_unique<entity::avenger>(textures);
            break;
        case aircraft::raptor:
            e = std::make_unique<entity::raptor>(textures);
            break;
        default:
            break;
        }

        e->setPosition(enemy_spawn_point.where);
        e->setRotation(180.f);

        layers[air]->attach(std::move(e));
    }
}

sf::FloatRect world_t::view_bounds() const
{
	return sf::FloatRect(view.getCenter() - view.getSize() / 2.f, view.getSize());
}

sf::FloatRect world_t::battlefield_bounds() const
{
	// Return view bounds + some area at top, where enemies spawn.
	sf::FloatRect bounds = view_bounds();
	bounds.top -= 100.f;
	bounds.height += 100.f;

	return bounds;
}

void world_t::update(
    sf::Time const dt)
{
    // Scroll the view.
    view.move(0.f, scroll_speed * dt.asSeconds());

    // Reset player velocity.
    player->velocity = {0.f, 0.f};

    // Dispatch commands.
    while(!commands_.empty())
    {
        graph.on_command(commands_.front(), dt);
        commands_.pop();
    }

    // Adjust player velocity if it is flying diagonally.
    auto const velocity = player->velocity;
    if(velocity.x != 0.f && velocity.y != 0.f)
    {
        player->velocity /= std::sqrt(2.f);
    }
    player->velocity += {0.f, scroll_speed};

    // Remove all destroyed entities, create new ones.
	spawn_enemies();

    // Update the entire graph.
    graph.update(dt);

    // Prevent the player from going off-screen.
    sf::FloatRect const bounds{view.getCenter() - view.getSize() / 2.f, view.getSize()};
    float const border_distance = 40.f;

    auto position = player->getPosition();
    position.x = std::clamp(position.x, bounds.left + border_distance, bounds.left + bounds.width - border_distance);
    position.y = std::clamp(position.y, bounds.top + border_distance, bounds.top + bounds.height - border_distance);

    player->setPosition(position);
}

void world_t::draw() const
{
    window.setView(view);
    window.draw(graph);
}

commands_t& world_t::commands()
{
    return commands_;
}