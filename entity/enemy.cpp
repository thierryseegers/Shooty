#include "entity/enemy.h"

#include "configuration.h"
#include "entity/flight.h"
#include "resources.h"
#include "utility.h"

#include <SFML/Graphics.hpp>

#include <vector>

namespace entity
{

std::vector<flight::direction> to_pattern(toml::array const& values)
{
    std::vector<flight::direction> pattern;

    for(auto const& node : values)
    {
        auto const& move = *node.as_array();
        pattern.push_back({*move[0].value<float>(), *move[1].value<float>()});
    }

    return pattern;
}

enemy::enemy(
    int const starting_life,
    int const speed,
    sf::Texture const& texture,
    std::vector<flight::direction> const& pattern)
    : aircraft_t{starting_life, texture}
    , speed{speed}
    , pattern{pattern}
    , current{this->pattern, this->pattern.begin()}
    , travelled{0}
{}

void enemy::update_self(
        sf::Time const& dt)
{
    if(travelled > current->distance)
    {
        ++current;
        travelled = 0;
    }

    float const rad = utility::to_radian(current->angle + 90.f);
    velocity = {speed * std::cos(rad), speed * std::sin(rad)};

    travelled += speed * dt.asSeconds();

    aircraft_t::update_self(dt);
}

avenger::avenger(
    resources::textures const& textures)
    : enemy{*configuration::instance()["avenger"]["starting_health"].value<int>(),
            *configuration::instance()["avenger"]["speed"].value<int>(),
            textures.get(resources::texture::avenger),
            to_pattern(*configuration::instance()["avenger"]["flight_pattern"].as_array())}
{}

raptor::raptor(
    resources::textures const& textures)
    : enemy{*configuration::instance()["raptor"]["starting_health"].value<int>(),
            *configuration::instance()["raptor"]["speed"].value<int>(),
            textures.get(resources::texture::raptor),
            to_pattern(*configuration::instance()["raptor"]["flight_pattern"].as_array())}
{}

}