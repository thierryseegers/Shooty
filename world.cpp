#include "world.h"

#include "command.h"
#include "configuration.h"
#include "entity/enemy.h"
#include "entity/leader.h"
#include "entity/missile.h"
#include "resources.h"
#include "scene.h"
#include "utility.h"

#include <SFML/Graphics.hpp>

#include <algorithm>
#include <array>
#include <cmath>
#include <memory>
#include <vector>

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
    auto& textures = utility::single::mutable_instance<resources::textures>();
    textures.load(resources::texture::desert, "Media/Textures/Desert.png");
    textures.load(resources::texture::bullet, "Media/Textures/Bullet.png");
    textures.load(resources::texture::missile, "Media/Textures/Missile.png");
    textures.load(resources::texture::avenger, *utility::single::instance<configuration::values>()["avenger"]["texture"].value<std::string>());
    textures.load(resources::texture::eagle, *utility::single::instance<configuration::values>()["leader"]["texture"].value<std::string>());
    textures.load(resources::texture::raptor, *utility::single::instance<configuration::values>()["raptor"]["texture"].value<std::string>());
}

void world_t::build_scene()
{
    // Create layers.
    auto air = std::make_unique<scene::air>();
    layers[layer::air] = air.get();
    graph.attach(std::move(air));
    
    auto background = std::make_unique<scene::background>();
    layers[layer::background] = background.get();
    graph.attach(std::move(background));

    // Create background sprite and move to background layer.
    sf::Texture& background_texture = utility::single::mutable_instance<resources::textures>().get(resources::texture::desert);
    background_texture.setRepeated(true);
    sf::IntRect background_rect{bounds};

    auto background_sprite = std::make_unique<scene::sprite_t>(background_texture, background_rect);
    background_sprite->setPosition(bounds.left, bounds.top);
    layers[layer::background]->attach(std::move(background_sprite));

    // Create leader aircraft and move to air layer.
    auto leader = std::make_unique<entity::leader_t>();
    player = leader.get();
    player->scene::sprite_t::setPosition(player_spawn_point);
    layers[layer::air]->attach(std::move(leader));

    auto add_enemy = [=](auto const& f, sf::Vector2f const& ds) mutable
        {
            enemy_spawns.insert({f, player_spawn_point + ds});
        };
    auto const make_raptor = []
        {
            return std::make_unique<entity::raptor>();
        };
    auto const make_avenger = []
        {
            return std::make_unique<entity::avenger>();
        };

    add_enemy(make_raptor,  {+0.,   -500.});
    add_enemy(make_raptor,  {+0.,   -1000.});
    add_enemy(make_raptor,  {+100., -1100.});
    add_enemy(make_raptor,  {-100., -1100.});
    add_enemy(make_avenger, {-70.,  -1400.});
    add_enemy(make_avenger, {-70.,  -1600.});
    add_enemy(make_avenger, {+70.,  -1400.});
    add_enemy(make_avenger, {+70.,  -1600.});
}

void world_t::spawn_enemies()
{
    while(!enemy_spawns.empty() &&
          enemy_spawns.rbegin()->where.y > battlefield_bounds().top)
    {
        auto const enemy_spawn = enemy_spawns.extract(*enemy_spawns.rbegin()).value();

        auto e = enemy_spawn.what();
        e->setPosition(enemy_spawn.where);
        e->setRotation(180.f);

        layers[air]->attach(std::move(e));
    }
}

void world_t::guide_missiles()
{
	// Setup command that stores all enemies in mActiveEnemies
    commands_.push(make_command<entity::enemy>([=](entity::enemy& e, sf::Time const&)
        {
            // if(!destroyed)
            enemies.push_back(&e);
        }));

    commands_.push(make_command<entity::guided_missile>([=](entity::guided_missile& gm, sf::Time const&)
        {
            if(!enemies.empty())
            {
                auto e =std::min_element(enemies.begin(), enemies.end(), [&](entity::enemy* e1, entity::enemy* e2)
                    {
                        return scene::distance(gm, *e1) < scene::distance(gm, *e2);
                    });

                gm.guide((*e)->world_position());
            }
        }));

    enemies.clear();
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

    // Guide guided missiles.
    guide_missiles();

    // Remove all destroyed entities, create new ones.
	spawn_enemies();

    // Update the entire graph.
    graph.update(dt, commands_);

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