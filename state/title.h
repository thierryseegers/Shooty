#include "state/stack.h"
#include "state/state.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

namespace state
{

class title : public state
{
public:
    title(
        stack& states);

    virtual void draw() override;

    virtual bool update(
        sf::Time const& dt) override;

    virtual bool handle_event(
        sf::Event const& event) override;

private:
    sf::Sprite background;
    sf::Text text;

    bool show_text;
    sf::Time blink_delay;
};

}
