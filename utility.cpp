#include "utility.h"

#include <SFML/Graphics.hpp>

#include <cmath>

void center_origin(sf::Sprite& sprite)
{
	auto const bounds = sprite.getLocalBounds();
	sprite.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
}

void center_origin(sf::Text& text)
{
	auto const bounds = text.getLocalBounds();
	text.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
}
