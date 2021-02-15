#include "lifebar.h"

#include "utility.h"

#include <SFML/Graphics.hpp>

#include <algorithm>
#include <cassert>

namespace entity
{

lifebar::lifebar(
    float const length,
    int const percent)
    : percent{percent}
{
    outline.setSize({length, std::max(10.f, length / 10.f)});
    outline.setOutlineThickness(2);
    outline.setOutlineColor(sf::Color::White);
    outline.setFillColor(sf::Color::Transparent);

    bar.setSize({outline.getLocalBounds().width - outline.getOutlineThickness() * 2,
                 outline.getLocalBounds().height - outline.getOutlineThickness() * 2});
    adjust(percent);

    utility::center_origin(outline);
    utility::center_origin(bar);
}

void lifebar::adjust(
    int const percent)
{
    assert(percent >= 0 && percent <= 100);

    if(percent >= 65)
    {
        bar.setFillColor(sf::Color::Green);
    }
    else if(percent >= 50)
    {
        bar.setFillColor(utility::blend(sf::Color::Green, sf::Color::Yellow, (65. - percent) / (65. - 50)));
    }
    else if(percent >= 25)
    {
        bar.setFillColor(sf::Color::Yellow);
    }
    else if(percent >= 10)
    {
        bar.setFillColor(utility::blend(sf::Color::Yellow, sf::Color::Red, (25. - percent) / (25. - 10)));
    }
    else
    {
        bar.setFillColor(sf::Color::Red);
    }

    bar.setSize({(outline.getLocalBounds().width - outline.getOutlineThickness() * 2) * (percent / 100.f),
                  bar.getSize().y});
}

void lifebar::draw_self(
    sf::RenderTarget& target,
    sf::RenderStates states) const
{
    target.draw(outline, states);
    target.draw(bar, states);
}

}
