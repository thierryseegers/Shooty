#include "configuration.h"

#include "tomlpp.h"

#include <filesystem>
#include <stdexcept>

namespace configuration
{

namespace
{

values& mutable_instance()
{
    static values v;
    return v;
}

}

values const& instance()
{
    return mutable_instance();
}

void initialize(
    std::filesystem::path const& config_file)
{
    static bool initialized = false;

    if(!initialized)
    {
        mutable_instance() = toml::parse_file(config_file.native());
        initialized = true;
    }
    else
    {
        throw std::logic_error("re-initialization of configuration");
    }
}

}