#include "configuration.h"

#include "tomlpp.h"
#include "utility.h"

#include <filesystem>
#include <stdexcept>

namespace configuration
{

void initialize(
    std::filesystem::path const& config_file)
{
    static bool initialized = false;

    if(!initialized)
    {
        utility::single::mutable_instance<values>() = toml::parse_file(config_file.native());
        initialized = true;
    }
    else
    {
        throw std::logic_error("re-initialization of configuration");
    }
}

}