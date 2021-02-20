#pragma once

#include "entity.h"
#include "projectile.h"
#include "scene.h"

namespace entity
{

class bullet : public projectile
{
public:
    bullet();

    virtual ~bullet() = default;
};

}
