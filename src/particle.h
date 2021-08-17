#pragma once

#include "command.h"
#include "configuration.h"
#include "resources.h"
#include "scene.h"
#include "utility.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

#include <deque>

struct particle
{
    sf::Time life_remaining;
    sf::Vector2f position;

    particle(
        sf::Time life_remaining,
        sf::Vector2f const& position)
        : life_remaining{life_remaining}
        , position{position}
    {}

    virtual ~particle() = default;

    virtual sf::Color const& color() const = 0;

    virtual sf::Time const& life_time() const = 0;
};

struct propellant : public particle
{
    propellant(
        sf::Vector2f const& position)
        : particle{sf::seconds(*configuration::values()["particle"]["propellant"]["lifetime"].value<float>()),
                   position}
    {}

    virtual ~propellant() = default;

    virtual sf::Color const& color() const override
    {
        static sf::Color const c = utility::to_color(*configuration::values()["particle"]["propellant"]["color"].as_array());
        return c;
    }

    virtual sf::Time const& life_time() const override
    {
        static sf::Time const t = sf::seconds(*configuration::values()["particle"]["propellant"]["lifetime"].value<float>());
        return t;
    }
};

struct smoke : public particle
{
    smoke(
        sf::Vector2f const& position)
        : particle{sf::seconds(*configuration::values()["particle"]["smoke"]["lifetime"].value<float>()),
                   position}
    {}

    virtual ~smoke() = default;

    virtual sf::Color const& color() const override
    {
        static sf::Color const c = utility::to_color(*configuration::values()["particle"]["smoke"]["color"].as_array());
        return c;
    }

    virtual sf::Time const& life_time() const override
    {
        static sf::Time const t = sf::seconds(*configuration::values()["particle"]["smoke"]["lifetime"].value<float>());
        return t;
    }
};

template<typename Particle>
class particles : public scene::node_t
{
public:
    particles(
        sf::Texture const& texture)
        : texture{texture}
        , vertices{sf::Quads}
        , update{true}
    {}

    virtual ~particles() = default;

    void add(
        sf::Vector2f const& position)
    {
        particles_.push_back(Particle{position});
    }

private:
    virtual void update_self(
        sf::Time const& dt,
        commands_t&) override
    {
        // Remove particles past their lifetime.
        particles_.erase(particles_.begin(), std::find_if_not(particles_.begin(), particles_.end(), [](particle const& p)
        {
            return p.life_remaining <= sf::Time::Zero;
        }));

        // Shorten lifetime of remaining particles.
        std::for_each(particles_.begin(), particles_.end(), [&](particle& p)
        {
            p.life_remaining -= dt;
        });

        update = true;
    }

    virtual void draw_self(
        sf::RenderTarget& target,
        sf::RenderStates states) const override
    {
        states.texture = &texture;

        if(update)
        {
            update = false;

            sf::Vector2f const size{states.texture->getSize()};
            sf::Vector2f const half = size / 2.f;

            vertices.clear();

            for(auto const& p : particles_)
            {
                float const color_ratio = p.life_remaining.asSeconds() / p.life_time().asSeconds();
                sf::Color const c = {p.color().r, p.color().g, p.color().b, static_cast<sf::Uint8>(255 * std::max(0.f, color_ratio))};

                vertices.append({{p.position.x - half.x, p.position.y - half.y}, c, {0.f, 0.f}});
                vertices.append({{p.position.x + half.x, p.position.y - half.y}, c, {size.x, 0.f}});
                vertices.append({{p.position.x + half.x, p.position.y + half.y}, c, {size.x, size.y}});
                vertices.append({{p.position.x - half.x, p.position.y + half.y}, c, {0.f, size.y}});
            }
        }

        target.draw(vertices, states);
    }

    sf::Texture const& texture;
    std::deque<Particle> particles_;

    mutable sf::VertexArray vertices;
    mutable bool update;
};

template<typename Particle>
class emitter : public scene::node_t
{
public:
    emitter()
        : remaining_time{sf::Time::Zero}
        , particles_{nullptr}
    {}

    virtual ~emitter() = default;

private:
    virtual void update_self(
        sf::Time const& dt,
        commands_t& commands) override
    {
        // This "finding" of the particles system node as a first step feels weird...
        if(particles_)
        {
            emit(dt);
        }
        else
        {
            commands.push(make_command<particles<Particle>>([=](particles<Particle>& p, sf::Time const&)
            {
                particles_ = &p;
            }));
        }
    }

    void emit(
        sf::Time const& dt)
    {
        float const emission_rate = 30.f;
        sf::Time const interval = sf::seconds(1.f) / emission_rate;

        remaining_time += dt;

        while(remaining_time > interval)
        {
            remaining_time -= interval;
            particles_->add(world_position());
        }
    }

    sf::Time remaining_time;
    particles<Particle> *particles_;
};
