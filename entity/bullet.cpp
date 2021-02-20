#include "bullet.h"

#include "configuration.h"
#include "resources.h"
#include "utility.h"

namespace entity
{

bullet::bullet()
    : projectile{*utility::single::instance<configuration::values>()["bullet"]["speed"].value<float>(),
                 *utility::single::instance<configuration::values>()["bullet"]["damage"].value<int>(),
                 utility::single::instance<resources::textures>().get(resources::texture::bullet)}
{}

}
