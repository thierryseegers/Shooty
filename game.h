#include <SFML/Graphics.hpp>

class game
{
public:
    game();

    void run();

private:
    void process_events();
    void update();
    void render();

    sf::RenderWindow window;
    sf::CircleShape player;
};