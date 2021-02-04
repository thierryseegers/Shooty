#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class game
{
public:
    game();

    void run();

private:
    void process_events();
    
    void handle_player_input(
        sf::Keyboard::Key const key,
        bool const is_pressed);
    
    void update(sf::Time const& dt);
    
    void render();

    sf::RenderWindow window;
    sf::CircleShape player;
    int player_speed = 100;

    bool moving_up, moving_down, moving_left, moving_right;

    sf::Time const time_per_frame = sf::seconds(1.f / 60.f);
};