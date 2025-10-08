#pragma once

#include <string>
#include "reproduction_strategy.hpp"

enum class TrophicCategory { 
    Producer, 
    Herbivore, 
    Carnivore, 
    Omnivore 
};

class Species{
    std::string name;
    TrophicCategory category;
    int lifespan;
    float max_energy;
    float sense_radius;
    float act_radius;
    float speed;
    std::vector<ReproductionStrategy> reproduction_strategies;
};