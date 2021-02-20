#pragma once

#include "command.h"
#include "entity.h"
#include "projectile.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

namespace entity
{

class missile : public projectile
{
public:
    missile();

    virtual ~missile() = default;
};

class guided_missile : public missile
{
public:
    guided_missile();

    virtual ~guided_missile() = default;

    void guide(sf::Vector2f const& position);

private:
    virtual void update_self(
        sf::Time const& dt,
        commands_t& commands) override;

    sf::Vector2f direction;
};

}
