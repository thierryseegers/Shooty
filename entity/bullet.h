#pragma once

#include "configuration.h"
#include "entity/entity.h"
#include "projectile.h"
#include "resources.h"
#include "utility.h"

namespace entity
{

template<template<class> class Kind>
class bullet : public Kind<projectile>
{
public:
    bullet()
        : Kind<projectile>{*utility::single::instance<configuration::values>()["bullet"]["speed"].value<float>(),
                           *utility::single::instance<configuration::values>()["bullet"]["damage"].value<int>(),
                           utility::single::instance<resources::textures>().get(resources::texture::bullet)}
    {}

    virtual ~bullet() = default;
};

}
