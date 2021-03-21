#pragma once

#include "effects/post_effect.h"

#include <SFML/Graphics/Glsl.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/System/Vector2.hpp>

#include <array>

namespace effect
{

class bloom : public post_effect
{
public:
    virtual ~bloom() = default;

    virtual void apply(
        sf::RenderTexture const& input,
        sf::RenderTarget& output) override;

private:
    using render_textures = std::array<sf::RenderTexture, 2>;

    void prepare_textures(
        sf::Vector2u const size);

    void filter_bright(
        sf::RenderTexture const& input,
        sf::RenderTexture& output);
    
    void blur_multipass(
        render_textures& render_textures);

    void blur(
        sf::RenderTexture const& input,
        sf::RenderTexture& output,
        sf::Glsl::Vec2 const offset_factor);
    
    void downsample(
        sf::RenderTexture const& input,
        sf::RenderTexture& output);

    void add(
        sf::RenderTexture const& source,
        sf::RenderTexture const& bloom,
        sf::RenderTarget& target);

    sf::RenderTexture brightness_texture;
    render_textures	first_pass_textures;
    render_textures	second_pass_textures;
};

}
