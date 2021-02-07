#include "world.h"

#include "aircraft.h"
#include "resources.h"
#include "scene.h"

#include <SFML/Graphics.hpp>

#include <memory>

world::world(
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

void world::load_textures()
{
    textures.load(resources::texture::desert, "Book/03_World/Media/Textures/Desert.png");
    textures.load(resources::texture::eagle, "Book/03_World/Media/Textures/Eagle.png");
    textures.load(resources::texture::raptor, "Book/03_World/Media/Textures/Raptor.png");
}

void world::build_scene()
{
    // Create layers.
    for(std::size_t i = 0; i != layer::count; ++i)
    {
        auto layer = std::make_unique<scene::node>();
        layers[i] = layer.get();

        graph.attach(std::move(layer));
    }

    // Create background sprite and move to `layers`.
    sf::Texture& background_texture = textures.get(resources::texture::desert);
    background_texture.setRepeated(true);
    sf::IntRect background_rect{bounds};

    auto background_sprite = std::make_unique<scene::sprite>(background_texture, background_rect);
    background_sprite->setPosition(bounds.left, bounds.top);
    layers[layer::background]->attach(std::move(background_sprite));

    // Create leader aircraft and move to `layers`.
    auto leader = std::make_unique<aircraft>(aircraft::type::eagle, textures);
    player = leader.get();
    player->setPosition(spawn_position);
    player->velocity = {40.f, scroll_speed};
    layers[layer::air]->attach(std::move(leader));

    // Create left and right escorts and attach to `leader`.
    auto left_escort = std::make_unique<aircraft>(aircraft::type::raptor, textures);
    left_escort->setPosition(-80.f, 50.f);
    player->attach(std::move(left_escort));

    auto right_escort = std::make_unique<aircraft>(aircraft::type::raptor, textures);
    right_escort->setPosition(80.f, 50.f);
    player->attach(std::move(right_escort));
}

void world::update(
    sf::Time const dt)
{
    // Scroll the view.
    view.move(0.f, scroll_speed * dt.asSeconds());

    // "Bounce" the planes if they get too close to the edge.
    auto const position = player->getPosition();
    if(position.x <= bounds.left + 150 ||
       position.x >= bounds.left + bounds.width - 150)
    {
        player->velocity.x = -player->velocity.x;
    }

    // Update the entire graph.
    graph.update(dt);
}

void world::draw() const
{
    window.setView(view);
    window.draw(graph);
}