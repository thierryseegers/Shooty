#pragma once

#include "configuration.h"
#include "entity/entity.h"
#include "projectile.h"
#include "resources.h"
#include "utility.h"

#include <magic_enum.hpp>
#include <SFML/Graphics/Rect.hpp>

namespace entity
{

template<template<class> class Kind>
class bullet : public Kind<projectile>
{};

template<>
class bullet<friendly> : public friendly<projectile>
{
public:
    bullet()
        : friendly<projectile>{*configuration::values()["projectile"]["bullet"]["speed"].value<float>(),
                               *configuration::values()["projectile"]["bullet"]["damage"].value<int>(),
                               *magic_enum::enum_cast<resources::texture>(*configuration::values()["projectile"]["texture"].value<std::string>()),
                               utility::to_intrect(*configuration::values()["projectile"]["bullet"]["friendly"]["texture_rect"].as_array())}
    {}

    virtual ~bullet<friendly>() = default;
};

template<>
class bullet<hostile> : public hostile<projectile>
{
public:
    bullet()
        : hostile<projectile>{*configuration::values()["projectile"]["bullet"]["speed"].value<float>(),
                               *configuration::values()["projectile"]["bullet"]["damage"].value<int>(),
                               *magic_enum::enum_cast<resources::texture>(*configuration::values()["projectile"]["texture"].value<std::string>()),
                               utility::to_intrect(*configuration::values()["projectile"]["bullet"]["hostile"]["texture_rect"].as_array())}
    {}

    virtual ~bullet<hostile>() = default;
};

}
