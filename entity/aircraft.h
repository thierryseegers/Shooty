#pragma once

#include "entity/entity.h"
#include "lifebar.h"
#include "scene.h"

#include <SFML/Graphics.hpp>

namespace entity
{

class aircraft_t : public entity, public scene::sprite_t
{
public:
    enum class type
    {
        eagle,
        raptor
    };

    explicit aircraft_t(
        type const type_,
        sf::Texture const& texture);

    virtual void update_self(
        sf::Time const& dt) override;

protected:
    type type_;

private:
    lifebar *bar;
};

struct leader_t : public aircraft_t
{
    using aircraft_t::aircraft_t;
};

}
