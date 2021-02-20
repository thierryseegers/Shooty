#pragma once

#include "entity/aircraft.h"
#include "resources.h"
#include "scene.h"

namespace entity
{

class leader_t : public aircraft_t
{
public:
    leader_t();

    virtual ~leader_t() = default;

    virtual void update_self(
        sf::Time const& dt,
        commands_t& commands) override;

    void fire();

    void launch_missile();

private:
    void shoot_bullet(
        scene::air& air) const;

    void shoot_missile(
        scene::air& air) const;

    int bullet_spread;
    int fire_rate;
    sf::Time fire_countdown;
    bool firing;

    int missile_ammo;
    bool missile_guidance;
    bool launching_missile;
};

}
