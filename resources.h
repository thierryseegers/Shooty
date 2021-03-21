#pragma once

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Shader.hpp>

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
    buttons, 
    entities,
    explosion,
    finish_line,
    jungle,
    particle,
    title_screen,
};

enum class shader_pass
{
    brightness,
    downsample,
    gaussian_blur,
    add,
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

using fonts_t = holder<font, sf::Font>;
using textures_t = holder<texture, sf::Texture>;
using shaders_t = holder<shader_pass, sf::Shader>;

fonts_t const& fonts();
textures_t const& textures();
shaders_t& shaders();

}
