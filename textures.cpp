#include "textures.h"

#include <SFML/Graphics.hpp>

#include <filesystem>
#include <map>
#include <memory>
#include <stdexcept>

void textures::holder::load(textures::id const id_, std::filesystem::path const& path)
{
    auto texture = std::make_unique<sf::Texture>();
    if(!texture->loadFromFile(path.native()))
    {
        throw std::runtime_error("textures::holder::load failed to load " + path.string());
    }

    textures[id_] = std::move(texture);
}

sf::Texture& textures::holder::get(textures::id const id_)
{
    auto const i = textures.find(id_);
    assert(i != textures.end());

    return *i->second;
}

sf::Texture const& textures::holder::get(textures::id const id_) const
{
    auto const i = textures.find(id_);
    assert(i != textures.end());

    return *i->second;
}
