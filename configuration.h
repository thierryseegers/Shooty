#pragma once

#include "tomlpp.h"

#include <filesystem>

namespace configuration
{

using values = toml::table;

values const& instance();

void initialize(
    std::filesystem::path const& config_file);

}
