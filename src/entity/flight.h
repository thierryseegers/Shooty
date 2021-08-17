#pragma once

#include <vector>

namespace flight
{

struct direction
{
    float angle;
    float distance;
};

using pattern = std::vector<direction>;

}
