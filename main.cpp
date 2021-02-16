#include "application.h"
#include "configuration.h"

#include <iostream>
#include <stdexcept>

int main(int, char**)
{
    try
    {
        configuration::initialize("./config.toml");

        application a;
        a.run();
    }
    catch(std::exception const& e)
    {
        std::cerr << "[exception] " << e.what() << std::endl;
    }

    return 0;
}
