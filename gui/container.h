#pragma once

#include "gui/component.h"

#include <SFML/Graphics.hpp>

#include <memory>
#include <vector>

namespace gui
{

class container : public component
{
    using children_t = std::vector<std::shared_ptr<component>>;

public:
    virtual ~container() = default;

    void pack(
        std::shared_ptr<component> component);

    [[nodiscard]] virtual bool selectable() const override;

    virtual void handle_event(
        sf::Event const& event) override;

private:
    virtual void draw(
        sf::RenderTarget& target,
        sf::RenderStates states) const override;

    enum class direction
    {
        next,
        previous
    };

    void select(
        direction const d);

private:
    children_t children;
    int selected = -1;
};

}
