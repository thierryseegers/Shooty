#pragma once

#include <SFML/Graphics.hpp>

#include <filesystem>
#include <map>
#include <memory>

namespace textures
{
    enum class id
    {
        landscape,
        airplane,
        missile
    };

    class holder
    {
    public:
        void load(id const id_, std::filesystem::path const& path);
        
        sf::Texture& get(id const id_);
        sf::Texture const& get(id const id_) const;

    private:
        std::map<id, std::unique_ptr<sf::Texture>> textures;
    };
}