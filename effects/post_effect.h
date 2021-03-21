#pragma once

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>

namespace effect
{

class post_effect
{
public:
    virtual ~post_effect() = default;

    virtual void apply(
        sf::RenderTexture const& input,
        sf::RenderTarget& output) = 0;

    static bool supported()
    {
        return sf::Shader::isAvailable();
    }

protected:
    static void apply_shader(
        sf::Shader const& shader,
        sf::RenderTarget& output)
    {
        sf::Vector2f const output_size = static_cast<sf::Vector2f>(output.getSize());

        sf::VertexArray vertices(sf::TrianglesStrip, 4);
        vertices[0] = sf::Vertex(sf::Vector2f{0, 0}, sf::Vector2f{0, 1});
        vertices[1] = sf::Vertex(sf::Vector2f{output_size.x, 0}, sf::Vector2f{1, 1});
        vertices[2] = sf::Vertex(sf::Vector2f{0, output_size.y}, sf::Vector2f{0, 0});
        vertices[3] = sf::Vertex(sf::Vector2f{output_size}, sf::Vector2f{1, 0});

        sf::RenderStates states;
        states.shader = &shader;
        states.blendMode = sf::BlendNone;

        output.draw(vertices, states);
    }
};

}
