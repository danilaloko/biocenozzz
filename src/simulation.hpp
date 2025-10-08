#pragma once

#include <toml++/toml.hpp>

class Simulation {
public:
    toml::table config;

    void run();
};