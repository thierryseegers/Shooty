#include "effects/bloom.h"

#include "resources.h"

#include <SFML/Graphics/Glsl.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/System/Vector2.hpp>

namespace effect
{

void bloom::apply(
        sf::RenderTexture const& input,
        sf::RenderTarget& output)
{
    prepare_textures(input.getSize());

    filter_bright(input, brightness_texture);

    downsample(brightness_texture, first_pass_textures[0]);
    blur_multipass(first_pass_textures);

    downsample(first_pass_textures[0], second_pass_textures[0]);
    blur_multipass(second_pass_textures);

    add(first_pass_textures[0], second_pass_textures[0], first_pass_textures[1]);
    first_pass_textures[1].display();
    add(input, first_pass_textures[1], output);
}

void bloom::filter_bright(
    sf::RenderTexture const& input,
    sf::RenderTexture& output)
{
    auto& brightness = resources::shaders().get(resources::shader_pass::brightness);

    brightness.setUniform("source", input.getTexture());
    apply_shader(brightness, output);
    output.display();
}

void bloom::blur_multipass(
    render_textures& render_textures)
{
    auto const texture_size = render_textures[0].getSize();

    for(std::size_t count = 0; count < 2; ++count)
    {
        blur(render_textures[0], render_textures[1], sf::Glsl::Vec2{0.f, 1.f / texture_size.y});
        blur(render_textures[1], render_textures[0], sf::Glsl::Vec2{1.f / texture_size.x, 0.f});
    }
}

void bloom::blur(
    sf::RenderTexture const& input,
    sf::RenderTexture& output,
    sf::Glsl::Vec2 const offset_factor)
{
    auto& gaussian_blur = resources::shaders().get(resources::shader_pass::gaussian_blur);

    gaussian_blur.setUniform("source", input.getTexture());
    gaussian_blur.setUniform("offsetFactor", offset_factor);
    apply_shader(gaussian_blur, output);
    output.display();
}

void bloom::downsample(
    sf::RenderTexture const& input,
    sf::RenderTexture& output)
{
    auto&  downsample = resources::shaders().get(resources::shader_pass::downsample);

    downsample.setUniform("source", input.getTexture());
    downsample.setUniform("sourceSize", sf::Vector2f{input.getSize()});
    apply_shader(downsample, output);
    output.display();
}

void bloom::add(
    sf::RenderTexture const& input,
    sf::RenderTexture const& bloom,
    sf::RenderTarget& target)
{
    auto& add = resources::shaders().get(resources::shader_pass::add);

    add.setUniform("source", input.getTexture());
    add.setUniform("bloom", bloom.getTexture());
    apply_shader(add, target);
}

void bloom::prepare_textures(
    sf::Vector2u const size)
{
    if(brightness_texture.getSize() != size)
    {
        brightness_texture.create(size.x, size.y);
        brightness_texture.setSmooth(true);

        first_pass_textures[0].create(size.x / 2, size.y / 2);
        first_pass_textures[0].setSmooth(true);
        first_pass_textures[1].create(size.x / 2, size.y / 2);
        first_pass_textures[1].setSmooth(true);

        second_pass_textures[0].create(size.x / 4, size.y / 4);
        second_pass_textures[0].setSmooth(true);
        second_pass_textures[1].create(size.x / 4, size.y / 4);
        second_pass_textures[1].setSmooth(true);
    }
}

}
