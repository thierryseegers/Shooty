#include "game.h"

#include <iostream>
#include <stdexcept>

int main(int, char**)
{
    try
    {
        game g;
        g.run();
    }
    catch(std::exception const& e)
    {
        std::cerr << "[exception] " << e.what() << std::endl;
    }

    return 0;
}
