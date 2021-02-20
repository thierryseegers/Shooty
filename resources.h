#pragma once

#include <SFML/Graphics.hpp>

#include <filesystem>
#include <map>
#include <memory>
#include <stdexcept>

namespace resources
{

enum class font
{
    label,
    main,
};

enum class texture
{
    avenger,
    bullet,
    button_normal, 
    button_pressed,
    button_selected,
    desert,
    eagle,
    health,
    increase_fire_rate,
    increase_spread,
    missile_refill,
    missile,
    raptor,
    title_screen,
};

template<typename Type, class Resource>
class holder
{
public:
    void load(
        Type const id,
        std::filesystem::path const& path)
    {
        auto resource = std::make_unique<Resource>();
        if(!resource->loadFromFile(path.native()))
        {
            throw std::runtime_error("resources::holder::load failed to load " + path.string());
        }

        resources[id] = std::move(resource);
    }
    
    template<typename Parameter>
    void load(
        Type const id,
        std::filesystem::path const& path, Parameter const& parameter)
    {
        auto resource = std::make_unique<Resource>();
        if(!resource->loadFromFile(path.native(), parameter))
        {
            throw std::runtime_error("resources::holder::load failed to load " + path.string());
        }

        resources[id] = std::move(resource);
    }

    Resource& get(
        Type const id)
    {
        auto const i = resources.find(id);
        assert(i != resources.end());

        return *i->second;
    }

    Resource const& get(
        Type const id) const
    {
        auto const i = resources.find(id);
        assert(i != resources.end());

        return *i->second;
    }

private:
    std::map<Type, std::unique_ptr<Resource>> resources;
};

using fonts = holder<font, sf::Font>;
using textures = holder<texture, sf::Texture>;

}
