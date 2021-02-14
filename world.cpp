#include "world.h"

#include "aircraft.h"
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
    , spawn_position{view.getSize().x / 2.f, bounds.height - view.getSize().y / 2.f}
    , scroll_speed{-50.f}
    , player{nullptr}
{
    load_textures();
    build_scene();

    view.setCenter(spawn_position);
}

void world_t::load_textures()
{
    textures.load(resources::texture::desert, "Book/06_Menus/Media/Textures/Desert.png");
    textures.load(resources::texture::eagle, "Book/06_Menus/Media/Textures/Eagle.png");
    textures.load(resources::texture::raptor, "Book/06_Menus/Media/Textures/Raptor.png");
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
    auto leader = std::make_unique<leader_t>(aircraft_t::type::eagle, textures.get(resources::texture::eagle));
    player = leader.get();
    player->scene::sprite_t::setPosition(spawn_position);
    layers[layer::air]->attach(std::move(leader));

    // Create left and right escorts and attach to leader.
    auto left_escort = std::make_unique<aircraft_t>(aircraft_t::type::raptor, textures.get(resources::texture::raptor));
    left_escort->scene::sprite_t::setPosition(-80.f, 50.f);
    player->scene::sprite_t::attach(std::move(left_escort));

    auto right_escort = std::make_unique<aircraft_t>(aircraft_t::type::raptor, textures.get(resources::texture::raptor));
    right_escort->scene::sprite_t::setPosition(80.f, 50.f);
    player->scene::sprite_t::attach(std::move(right_escort));
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