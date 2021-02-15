#pragma once

#include <SFML/Graphics.hpp>

#include <iterator>
#include <string>

namespace utility
{

// Change origin of object to be its center rather than its top-left corner.
void center_origin(
    sf::Transformable& t);
void center_origin(
    sf::Text& text);
void center_origin(
    sf::Shape& shape);

// Blends two color together by a given ratio [0., 1.].
sf::Color blend(
    sf::Color const& a,
    sf::Color const& b,
    float const ratio);

// Converts a keyboard key to its std::string representation.
std::string to_string(
    sf::Keyboard::Key const key);

// An iterator that cyles forward or barckward when it reachs the end or the beginning of its container.
template<class Container>
struct cyclical_iterator 
{
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type   = typename Container::iterator::difference_type;
    using value_type        = typename Container::iterator::value_type;
    using pointer           = typename Container::iterator::pointer;
    using reference         = typename Container::iterator::reference;

    cyclical_iterator(
        Container& container,
        typename Container::iterator i)
        : container{container}
        , i{i}
    {}

    typename Container::iterator base()
    {
        return i;
    }

    reference operator*() const
    {
        return *i;
    }

    pointer operator->()
    {
        return i;
    }
    
    cyclical_iterator& operator++()
    {
        if(i == std::prev(container.end()))
        {
            i = container.begin();
        }
        else
        {
            std::advance(i, 1);
        }

        return *this;
    }

    cyclical_iterator operator++(int)
    {
        cyclical_iterator tmp = *this;
        ++(*this);
        return tmp;
    }

    cyclical_iterator& operator--()
    {
        if(i == container.begin())
        {
            i = std::prev(container.end());
        }
        else
        {
            std::advance(i, -1);
        }

        return *this;
    }

    cyclical_iterator operator--(int)
    {
        cyclical_iterator tmp = *this;
        --(*this);
        return tmp;
    }

    friend bool operator==(
        const cyclical_iterator& a,
        const cyclical_iterator& b)
    { 
        return &a.container == &b.container && a.i == b.i;
    }

    friend bool operator!=(
        const cyclical_iterator& a,
        const cyclical_iterator& b)
    {
        return &a.container != &b.container || a.i != b.i;
    }

private:
    Container& container;
    typename Container::iterator i;
};

}
