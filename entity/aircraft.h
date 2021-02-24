#pragma once

#include "command.h"
#include "entity/entity.h"
#include "lifebar.h"
#include "projectile.h"
#include "scene.h"

#include <SFML/Graphics.hpp>

namespace entity
{

class aircraft_t : public entity
{
public:
    explicit aircraft_t(
        int const starting_life,
        sf::Texture const& texture);

    virtual ~aircraft_t() = default;

    virtual void damage(
        int const amount);

    [[nodiscard]] int health() const
    {
        return life;
    }

protected:
    virtual void update_self(
        sf::Time const& dt,
        commands_t& commands) override;

    template<typename Projectile>
    void add_projectile(
        scene::air& air,
        sf::Vector2f const& offset,
        float const& dir) const
    {
        auto p = std::make_unique<Projectile>();

        sf::Vector2f const o{offset.x * sprite.getGlobalBounds().width,
                             offset.y * sprite.getGlobalBounds().height};

        if(dir == projectile::downward)
        {
            p->setRotation(180.f);
        }

        p->setPosition(world_position() + o * dir);
        p->velocity = sf::Vector2f{0, p->speed} * dir;

        air.attach(std::move(p));
    }

    int const starting_life;
    int life;

    lifebar *bar;
};

}
