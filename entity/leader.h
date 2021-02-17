#pragma once

#include "entity/aircraft.h"
#include "resources.h"

namespace entity
{

struct leader_t : public aircraft_t
{
    leader_t(
        resources::textures const& textures);
};

}
