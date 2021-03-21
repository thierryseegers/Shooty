#include "resources.h"

#include "utility.h"

namespace resources
{

fonts_t const& fonts()
{
    return utility::single::instance<fonts_t>();
}

textures_t const& textures()
{
    return utility::single::instance<textures_t>();
}

shaders_t& shaders()
{
    return utility::single::mutable_instance<shaders_t>();
}

}
